#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include <AsynchronousJobManager.h>
#include "mocks/MockJob.h"
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


TEST(AsynchronousJobManagerTest, test_only_one_job_allowed) {
    workflow::Workflow workflow;
    auto identifier = workflow.add_step("step", {}, {});
    auto step = std::make_shared<mimo::MockStep>();
    mimo::AsynchronousJobManager manager(identifier, step);
    auto wrong_job = std::make_shared<mimo::MockJob>();

    EXPECT_TRUE(manager.has_runnable_job());
    auto job = manager.get_runnable_job();
    EXPECT_FALSE(manager.has_runnable_job());
    EXPECT_THROW(manager.return_runnable_job(wrong_job), std::runtime_error);
    manager.return_runnable_job(job);
    EXPECT_TRUE(manager.has_runnable_job());
}
