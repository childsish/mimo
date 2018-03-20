#include "queues/JobInputs.h"
#include "queues/JobOutputs.h"
#include "Job.h"
#include "JobFactory.h"

mimo::JobFactory::JobFactory(const std::shared_ptr<IJobInputsFactory> inputs_factory,
                             const std::shared_ptr<IJobOutputsFactory> outputs_factory) :
    inputs_factory(inputs_factory),
    outputs_factory(outputs_factory) {}

mimo::IJob *
mimo::JobFactory::make_raw(const std::shared_ptr<workflow::Step> &step_id,
                           std::shared_ptr<Step> step) const {
    return new Job(step_id, std::move(step), this->inputs_factory, this->outputs_factory);
}
