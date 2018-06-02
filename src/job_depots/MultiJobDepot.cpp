#include <algorithm>
#include <workflow/Step.h>
#include "MultiJobDepot.h"
#include "ISingleJobDepot.h"
#include "../queues/IQueueBundle.h"
#include "../IJob.h"


mimo::MultiJobDepot::MultiJobDepot(
    std::shared_ptr<workflow::Workflow> workflow_,
    std::shared_ptr<ISingleJobDepotFactory> factory
) :
    workflow_(workflow_)
{
    for (const auto &&step_id : workflow_->get_steps()) {
        this->depots.emplace(step_id.second, std::move(factory->make_depot(step_id.second)));
        if (this->depots.at(step_id.second)->has_runnable_jobs()) {
            this->runnable_jobs.insert(step_id.second);
        }
    }
}

void mimo::MultiJobDepot::push(
    const std::shared_ptr<workflow::Input> &input_id,
    std::shared_ptr<mimo::Entity> entity
) {
    auto &&step_id = this->workflow_->get_connected_step(input_id);
    this->depots[step_id]->push(input_id, entity);
    auto jobs = this->depots.at(step_id)->get_runnable_jobs();
    this->runnable_jobs.insert(jobs.begin(), jobs.end());
}

bool mimo::MultiJobDepot::can_queue(const std::shared_ptr<workflow::Output> &output_id) {
    const auto &&input_ids = this->workflow_->get_connected_inputs(output_id);
    return std::all_of(
        input_ids.begin(),
        input_ids.end(),
        [this](const std::shared_ptr<workflow::Input> &&input_id) {
            return this->depots.at(this->workflow_->get_connected_step(input_id))->can_queue(input_id);
        }
    );
}

void mimo::MultiJobDepot::queue_input(
    const std::shared_ptr<workflow::Output> &output_id,
    const IQueue &queue
) {
    for (auto &&input_id : this->workflow_->get_connected_inputs(output_id)) {
        this->depots.at(this->workflow_->get_connected_step(input_id))->queue_input(input_id, queue);
    }
}

bool mimo::MultiJobDepot::has_runnable_jobs() const {
    return !this->runnable_jobs.empty();
}

std::set<std::unique_ptr<mimo::IJob>> mimo::MultiJobDepot::get_runnable_jobs() {
    std::set<std::unique_ptr<IJob>> jobs;
    for (const auto &&step_id : this->runnable_jobs) {
        for (const auto &&job : this->depots.at(step_id)->get_runnable_jobs()) {
            jobs.emplace(std::move(job));
        }
    }
    this->runnable_jobs.clear();
    return jobs;
}

void mimo::MultiJobDepot::return_exhausted_job(std::unique_ptr<mimo::IJob> job) {
    auto &&step_id = job->get_step_id();
    this->depots.at(step_id)->return_exhausted_job(std::move(job));
    if (this->depots.at(step_id)->has_runnable_jobs()) {
        this->runnable_jobs.insert(step_id);
    }
}
