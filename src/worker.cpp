/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */


#include <algorithm>
#include <iostream>
#include <mutex>
#include <queue>
#include <unordered_set>
#include <workflow/Workflow.h>
#include "Job.h"
#include "JobManager.h"
#include "queues/QueueChannel.h"


workflow::Workflow workflow_;
mimo::JobManager job_manager;

std::unordered_map<unsigned int, std::unique_ptr<mimo::Queue>> inputs;
std::unordered_map<unsigned int, mimo::QueueChannel> outputs;

std::queue<mimo::Job> before_run;
std::mutex before_run_mutex;
std::queue<mimo::Job> after_run;
std::mutex after_run_mutex;

std::queue<unsigned int> ready_outputs;
std::mutex ready_outputs_mutex;


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
void process_ready_outputs() {
    // TODO: Ensure popped output identifiers are unique
    ready_outputs_mutex.lock();
    unsigned int output_id = ready_outputs.front(); // TODO: get correct output identifier
    ready_outputs.pop();
    ready_outputs_mutex.unlock();

    auto connected_inputs = workflow_.get_connected_inputs(output_id);
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

    std::vector<std::shared_ptr<workflow::Step>> connected_steps;
    for (auto &connected_input : connected_inputs) {
        for (auto &connected_step : workflow_.get_connected_steps(connected_input)) {
            connected_steps.push_back(connected_step);
        }
    }

    for (auto &connected_step : connected_steps) {
        connected_inputs = workflow_.get_connected_inputs(connected_step);
        if (std::all_of(
                connected_inputs.begin(), connected_inputs.end(),
                [](const std::unordered_map<unsigned int, std::unique_ptr<mimo::Queue>>::const_iterator &item){
                    return item->second->can_pop();
                }
        ))
        {
            if (job_manager.can_make_job()) {
                before_run.push(job_manager.make_job());
            }
        }
    }
}

/**
 * Client loop for processing jobs.
 */
void worker() {
    while (!before_run.empty() && !after_run.empty()) {
        if (!before_run.empty()) {
            process_before_job();
        }
        else if (!after_run.empty()) {
            process_after_job();
        }
        else if (!ready_outputs.empty()) {
            process_ready_outputs();
        }
    }
}
