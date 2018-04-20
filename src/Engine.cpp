#include <workflow/Output.h>
#include "mimo/Engine.h"
#include "IJob.h"
#include "job_depots/IMultiJobDepot.h"
#include "job_depots/MultiJobDepotFactory.h"
#include "queues/QueueBundle.h"

#include <iostream>

mimo::Engine::Engine(unsigned int capacity, std::shared_ptr<IJobFactory> factory) :
    factory(std::make_shared<MultiJobDepotFactory>(
        std::make_shared<SingleJobDepotFactory>(capacity, std::move(factory)))
    ) {}

mimo::Engine::Engine(std::shared_ptr<IMultiJobDepotFactory> factory) : factory(std::move(factory)) {}

void mimo::Engine::register_step(const std::shared_ptr<workflow::Step> &identifier, std::shared_ptr<Step> step) {
    this->factory->register_step(identifier, std::move(step));
}

void mimo::Engine::run(std::shared_ptr<workflow::Workflow> workflow) {
    auto depot = this->factory->make_depot(workflow);
    while (depot->has_runnable_job()) {
        auto job = depot->get_runnable_job();
        while (job->can_run()) {
            job->run();
            auto outputs = workflow->get_connected_outputs(job->get_step_id());
            for (auto &output : outputs) {
                auto bundle = job->get_outputs();
                depot->add_entity(output, bundle->pop(output->name));
            }
        }
        depot->return_complete_job(job);
    }
}
