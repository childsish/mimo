#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include "../src/job_depots/IJobDepot.h"
#include "mocks/MockStep.h"
#include "../src/job_depots/SingleJobDepot.h"

TEST(JobManagerFactoryTest, test_register_and_create_step) {
    workflow::Workflow workflow;
    auto identifier1 = workflow.add_step("step", {"input1", "input2"}, {});
    identifier1->synchronise_inputs({"input1", "input2"});
    auto identifier2 = workflow.add_step("step", {"input1", "input2"}, {});
    auto step1 = std::make_shared<mimo::MockStep>();
    auto step2 = std::make_shared<mimo::MockStep>();

    mimo::SingleJobDepotFactory factory(1);
    factory.register_step(identifier1, step1);
    factory.register_step(identifier2, step2);
    auto asynchronous_manager = factory.make_depot(identifier1);
    auto synchronous_manager = factory.make_depot(identifier2);
}
