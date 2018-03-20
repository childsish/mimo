/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include "gtest/gtest.h"
#include <workflow/Workflow.h>
#include "IJobManager.h"
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "JobManagerFactory.h"

TEST(JobManagerFactoryTest, test_register_and_create_step) {
    workflow::Workflow workflow;
    auto step1 = workflow.add_step("step", {"input1", "input2"}, {});
    step1->synchronise_inputs({"input1", "input2"});
    auto step2 = workflow.add_step("step", {"input1", "input2"}, {});

    mimo::JobManagerFactory factory(1);
    factory.register_step<mimo::MockStep>(step1);
    factory.register_step<mimo::MockStep>(step2);
    auto asynchronous_manager = factory.make_manager(step1);
    auto synchronous_manager = factory.make_manager(step2);
}
