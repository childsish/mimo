#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include "../src/IJob.h"
#include "../src/JobFactory.h"
#include "mocks/MockStep.h"
#include "mimo/IInputs.h"
#include "mimo/IOutputs.h"

TEST(JobFactoryTest, test_make_job) {;
    workflow::Workflow workflow;
    auto identifier = workflow.add_step("step", {}, {});
    auto step = std::make_shared<mimo::MockStep>();
    mimo::JobFactory factory;
    auto job = factory.make_unique(identifier, step);

    EXPECT_EQ(job->get_step_id(), identifier);
}
