#include "MultiJobDepot.h"

#include <algorithm>
#include <workflow/Step.h>
#include "../JobComparator.h"


mimo::MultiJobDepot::MultiJobDepot(
    std::shared_ptr<workflow::Workflow> workflow_,
    std::shared_ptr<ISingleJobDepotFactory> factory
) :
    workflow_(std::move(workflow_))
{
    for (const auto step_id : this->workflow_->get_steps()) {
        this->depots.emplace(step_id.second, std::move(factory->make_unique(step_id.second)));
        if (this->depots.at(step_id.second)->has_runnable_jobs()) {
            this->runnable_jobs.insert(step_id.second);
        }
    }
}

void mimo::MultiJobDepot::push(
    const workflow::Input &input_id,
    std::shared_ptr<mimo::Entity> entity
) {
    auto &step_id = this->workflow_->get_connected_step(input_id);
    this->depots.at(step_id)->push(input_id, entity);
    if (this->depots.at(step_id)->has_runnable_jobs()) {
        this->runnable_jobs.insert(step_id);
    }
}

bool mimo::MultiJobDepot::can_queue(const workflow::Output &output_id) {
    const auto &input_ids = this->workflow_->get_connected_inputs(output_id);
    return std::all_of(
        input_ids.begin(),
        input_ids.end(),
        [this](const std::shared_ptr<workflow::Input> &input_id) {
            const auto &step_id = this->workflow_->get_connected_step(*input_id);
            return this->depots.at(step_id)->can_queue(*input_id);
        }
    );
}

void mimo::MultiJobDepot::queue_input(
    const workflow::Output &output_id,
    const IQueue &queue
) {
    for (auto &&input_id : this->workflow_->get_connected_inputs(output_id)) {
        const auto &step_id = this->workflow_->get_connected_step(*input_id);
        auto &depot = this->depots.at(step_id);
        depot->queue_input(*input_id, queue);
        if (depot->has_runnable_jobs()) {
            this->runnable_jobs.insert(step_id);
        }
    }
}

bool mimo::MultiJobDepot::has_runnable_jobs() const {
    return !this->runnable_jobs.empty();
}

std::vector<std::unique_ptr<mimo::IJob>> mimo::MultiJobDepot::get_runnable_jobs() {
    std::vector<std::unique_ptr<IJob>> jobs;
    for (const auto &step_id : this->runnable_jobs) {
        for (auto &&job : this->depots.at(step_id)->get_runnable_jobs()) {
            jobs.emplace_back(std::move(job));
        }
    }
    this->runnable_jobs.clear();
    std::sort(jobs.begin(), jobs.end(), JobComparator());
    return jobs;
}

void mimo::MultiJobDepot::return_exhausted_job(std::unique_ptr<mimo::IJob> job) {
    const auto &step_id = job->get_step_id();
    auto &depot = this->depots.at(step_id);
    depot->return_exhausted_job(std::move(job));
    if (depot->has_runnable_jobs()) {
        this->runnable_jobs.insert(step_id);
    }
}
