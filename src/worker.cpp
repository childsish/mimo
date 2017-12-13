/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */


#include <algorithm>
#include <mutex>
#include <queue>
#include <unordered_set>
#include <workflow/Workflow.h>
#include "Job.h"
#include "queues/QueueChannel.h"


std::queue<mimo::Job> before_run;
std::mutex before_run_mutex;
std::queue<mimo::Job> after_run;
std::mutex after_run_mutex;

void create_job(workflow::Workflow workflow,
                const std::shared_ptr<workflow::Step> &step,
                std::unordered_map<unsigned int, mimo::QueueChannel> inputs) {
    for (auto input_name : workflow.get_connected_inputs(step)) {
        inputs[input_name].pop();
    }
}

void drain_outputs(
        mimo::Outputs &outputs,
        const workflow::Workflow &workflow,
        std::unordered_map<unsigned int, mimo::QueueChannel> inputs
) {
    for (auto output : outputs) {
        auto inputs = workflow.get_connected_inputs(output);
        for (auto input : inputs) {

        }
    }
}

bool can_run(
        const std::shared_ptr<workflow::Step> &step,
        const workflow::Workflow &workflow,
        std::unordered_map<unsigned int, mimo::QueueChannel> inputs,
        std::unordered_map<unsigned int, mimo::QueueChannel> outputs
) {
    auto step_inputs = workflow.get_connected_inputs(step);
    auto step_outputs = workflow.get_connected_outputs(step);

    return std::all_of(step_inputs.begin(), step_inputs.end(),
                       [&inputs](std::shared_ptr<workflow::Input> input){
                           return inputs[input->identifier].can_pop();
                       })
            && std::all_of(step_outputs.begin(), step_outputs.end(),
                           [&outputs](std::shared_ptr<workflow::Output> output){
                               return outputs[output->identifier].can_reserve();
                           });
}

std::vector<mimo::Job> get_next_jobs(
        const mimo::Job &job,
        const workflow::Workflow &workflow,
        std::unordered_map<unsigned int, mimo::QueueChannel> inputs,
        std::unordered_map<unsigned int, mimo::QueueChannel> outputs
) {
    auto step_outputs = workflow.get_connected_outputs(job.step.identifier);
    std::unordered_set<std::shared_ptr<workflow::Step>> next_step_candidates;
    for (auto output : step_outputs) {
        step_outputs.merge(workflow.get_connected_steps(output));
    }

    std::unordered_set<std::shared_ptr<workflow::Step>> next_steps;
    for (auto candidate : next_step_candidates) {
        if (can_run(candidate, workflow, inputs, outputs)) {
            next_steps.insert(candidate);
        }
    }
}

void drain_outputs(
        const workflow::Workflow &workflow,
        std::unordered_map<unsigned int, mimo::QueueChannel> inputs,
        std::unordered_map<unsigned int, mimo::QueueChannel> outputs
) {
    if (std::all_of(output.can_push(output))) {
        for (auto &output : job.outputs) {
            unsigned int fragment = output->run + 1;
            outputs[output->id].push(std::move(output));
            outputs[output->id] = std::make_unique<mimo::Queue>(fragment);
        }
    }
    else {
        before_run.push(job);
    }
}

void worker(
        const workflow::Workflow &workflow,
        std::unordered_map<unsigned int, mimo::QueueChannel> inputs,
        std::unordered_map<unsigned int, mimo::QueueChannel> outputs
) {
    while (!before_run.empty() && !after_run.empty()) {
        if (!before_run.empty()) {
            before_run_mutex.lock();
            mimo::Job job = before_run.front();
            before_run.pop();
            before_run_mutex.unlock();

            job.run();

            if (job.completed) {
                for (auto &output : job.outs) {
                    output.second.end_run();
                }
                if (job.ins.is_empty() || job.ins.is_closed()) {
                    for (auto &output : job.outs) {
                        output.second.close();
                    }
                }
            }

            after_run_mutex.lock();
            after_run.push(job);
            after_run_mutex.unlock();
        }

        if (!after_run.empty()) {
            after_run_mutex.lock();
            mimo::Job job = after_run.front();
            after_run.pop();
            after_run_mutex.unlock();

            drain_outputs(workflow, outputs);
            auto next_jobs = get_next_jobs(job, workflow, inputs, outputs);
            for (auto next_job : next_jobs) {
                before_run_mutex.lock();
                before_run.push(next_job);
                before_run_mutex.unlock();
            }
            if (!job.completed) {
                before_run_mutex.lock();
                before_run.push(job);
                before_run_mutex.unlock();
            }
        }
    }
}
