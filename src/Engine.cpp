#include "mimo/Engine.h"
#include "IJob.h"
#include "job_depots/IJobDepot.h"
#include "job_depots/MultiJobDepotFactory.h"

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
    auto depot = this->factory->make_depot(std::move(workflow));
    while (depot->has_runnable_job()) {
        auto job = depot->get_runnable_job();
        while (job->can_run()) {
            job->run();
            job->get_outputs();
        }
        depot->return_complete_job(job);
    }
}
