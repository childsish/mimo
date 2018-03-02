/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include "gtest/gtest.h"
#include <workflow/Workflow.h>
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "StepFactory.h"

TEST(StepFactoryTest, test_register_and_create_step) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input"}, {"output"});
    mimo::StepFactory factory;

    factory.register_step<mimo::MockStep>(step);
    std::unique_ptr<mimo::Step> generic_step(factory.make_step(step));
}
