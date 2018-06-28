#include <workflow/Output.h>
#include "mimo/Engine.h"
#include "IJob.h"
#include "depots/IMultiJobDepot.h"
#include "depots/MultiJobDepotFactory.h"
#include "queues/QueueBundle.h"

#include <iostream>

mimo::Engine::Engine(std::shared_ptr<IMultiJobDepotFactory> factory) :
    factory(std::move(factory)) {}

void mimo::Engine::register_step(
    std::shared_ptr<workflow::Step> step_id,
    std::shared_ptr<Step> step
) {
    this->factory->register_step(std::move(step_id), std::move(step));
}

void mimo::Engine::run(std::shared_ptr<workflow::Workflow> workflow) {
    auto depot = this->factory->make_unique(workflow);
    while (depot->has_runnable_jobs()) {
        for (auto &job : depot->get_runnable_jobs()) {
            job->run();
            auto outputs = job->get_outputs();
            for (auto &output_id : workflow->get_connected_outputs(*job->get_step_id())) {
                auto &queue = outputs->get_queue(output_id->name);
                depot->queue_input(*output_id, queue);
                queue.clear();
            }
            depot->return_exhausted_job(std::move(job));
        }
    }
}
