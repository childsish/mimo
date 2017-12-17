/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */


#include <algorithm>
#include <mutex>
#include <queue>
#include <unordered_set>
#include <workflow/Workflow.h>
#include <iostream>
#include "Job.h"
#include "queues/QueueChannel.h"

std::unordered_map<unsigned int, std::unique_ptr<mimo::Queue>> inputs;
std::unordered_map<unsigned int, mimo::QueueChannel> outputs;

std::queue<mimo::Job> before_run;
std::mutex before_run_mutex;
std::queue<mimo::Job> after_run;
std::mutex after_run_mutex;

std::queue<unsigned int> ready_outputs;
std::mutex ready_outputs_mutex;


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


void process_global_output() {
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

/**
 * Run the job and end/close output if job completed/closed.
 */
void process_before_job() {
    before_run_mutex.lock();
    mimo::Job job = before_run.front();
    before_run.pop();
    before_run_mutex.unlock();

    job.run();

    if (job.completed) {
        job.outs.end_run();
        if (job.ins.is_empty() || job.ins.is_closed()) {
            job.outs.close();
        }
    }

    after_run_mutex.lock();
    after_run.push(job);
    after_run_mutex.unlock();
}

/**
 * Move job output to global output
 */
void process_after_job() {
    after_run_mutex.lock();
    mimo::Job job = after_run.front();
    after_run.pop();
    after_run_mutex.unlock();

    for (auto &output : job.outs) {
        if (output.second.is_empty()) {
            continue;
        }
        unsigned int output_id = 0; // TODO: get correct output identifier
        unsigned int run = 0; // TODO: get run identifier
        outputs[output_id].lock();
        mimo::QueueChannel::PushStatus push_status = outputs[output_id].get_push_status(run);
        if (push_status == mimo::QueueChannel::CAN_PUSH) {
            outputs[output_id].push(output.second.release_queue());

            ready_outputs_mutex.lock();
            ready_outputs.push(output_id);
            ready_outputs_mutex.unlock();
        }
        else if (push_status == mimo::QueueChannel::PUSH_ENDED) {
            // TODO: replace with proper logging
            outputs[output_id].unlock();
            std::cout << "Error: attempting to push job queue after job's last queue already pushed." << std::endl;
            throw std::runtime_error("Error: attempting to push job queue after job's last queue already pushed.");
        }
        outputs[output_id].unlock();
    }

    if (job.outs.is_empty()) {
        if (!job.completed) {
            before_run_mutex.lock();
            before_run.push(job);
            before_run_mutex.unlock();
        }
    }
    else {
        after_run_mutex.lock();
        after_run.push(job);
        after_run_mutex.unlock();
    }
}

/**
 * Move global outputs to global inputs and queue next jobs
 */
void process_ready_outputs(const workflow::Workflow &workflow) {
    // TODO: Ensure popped output identifiers are unique
    ready_outputs_mutex.lock();
    unsigned int output_id = ready_outputs.front(); // TODO: get correct output identifier
    ready_outputs.pop();
    ready_outputs_mutex.unlock();

    auto connected_inputs = workflow.get_connected_inputs(output_id);
    while (std::all_of(
            connected_inputs.begin(), connected_inputs.end(),
            [](const std::unordered_map<unsigned int, std::unique_ptr<mimo::Queue>>::const_iterator &item){
                return item->second->can_push();
            }) &&
            outputs[output_id].peek()->can_pop())
    {
        auto entity = outputs[output_id].peek()->pop();
        for (auto &input : connected_inputs) {
            inputs[input->identifier]->push(entity);
        }
    }
}

void worker(const workflow::Workflow &workflow) {
    while (!before_run.empty() && !after_run.empty()) {
        if (!before_run.empty()) {
            process_before_job();
        }
        else if (!after_run.empty()) {
            process_after_job();
        }
        else if (!ready_outputs.empty()) {
            process_ready_outputs(workflow);
        }
    }
}
