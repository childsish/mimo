#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include "IJob.h"
#include "JobFactory.h"
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"

TEST(JobFactoryTest, test_make_job) {;
    workflow::Workflow workflow;
    auto identifier = workflow.add_step("step", {}, {});
    auto step = std::make_shared<mimo::MockStep>();
    mimo::JobFactory factory;
    auto job = factory.make_unique(identifier, step);

    EXPECT_EQ(job->get_identifier(), identifier);
}
