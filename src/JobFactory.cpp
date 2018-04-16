#include "Job.h"
#include "JobFactory.h"

mimo::JobFactory::JobFactory(const std::shared_ptr<IQueueBundleFactory> factory) :
    factory(factory) {}

mimo::IJob *mimo::JobFactory::make_raw(
    const std::shared_ptr<workflow::Step> &step_id,
    std::shared_ptr<Step> step) const
{
    return new Job(step_id, std::move(step), this->factory);
}
