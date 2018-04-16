#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include "job_depots/IJobDepot.h"
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "job_depots/SingleJobDepotFactory.h"

TEST(JobManagerFactoryTest, test_register_and_create_step) {
    workflow::Workflow workflow;
    auto step1 = workflow.add_step("step", {"input1", "input2"}, {});
    step1->synchronise_inputs({"input1", "input2"});
    auto step2 = workflow.add_step("step", {"input1", "input2"}, {});

    mimo::SingleJobDepotFactory factory(1);
    factory.register_step(step1, [](){ return std::make_shared<mimo::MockStep>(); });
    factory.register_step(step2, [](){ return std::make_shared<mimo::MockStep>(); });
    auto asynchronous_manager = factory.make_manager(step1);
    auto synchronous_manager = factory.make_manager(step2);
}
