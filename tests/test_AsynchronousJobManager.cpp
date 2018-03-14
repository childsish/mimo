#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include <AsynchronousJobManager.h>
#include "mocks/MockJob.h"
#include "mocks/MockJobFactory.h"
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


using ::testing::Return;

TEST(AsynchronousJobManagerTest, test_only_one_job_allowed) {
    workflow::Workflow workflow;

    auto job_proxy = new mimo::MockJob();
    EXPECT_CALL(*job_proxy, can_run())
        .WillOnce(Return(false))
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(true));
    auto identifier = workflow.add_step("step1", {}, {});
    EXPECT_CALL(*job_proxy, get_step_id())
        .WillOnce(Return(identifier))
        .WillOnce(Return(identifier))
        .WillOnce(Return(identifier));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_shared_proxy(identifier, step))
        .WillOnce(Return(job_proxy));

    mimo::AsynchronousJobManager manager(identifier, step, factory);

    EXPECT_FALSE(manager.has_runnable_job());
    EXPECT_TRUE(manager.has_runnable_job());
    auto job = manager.get_runnable_job();
    EXPECT_FALSE(manager.has_runnable_job());

    auto wrong_job = std::make_shared<mimo::MockJob>();
    auto wrong_identifier = workflow.add_step("step2", {}, {});
    EXPECT_CALL(*wrong_job, get_step_id())
        .WillOnce(Return(wrong_identifier));
    EXPECT_THROW(manager.return_complete_job(wrong_job), std::runtime_error);

    manager.return_complete_job(job);
    EXPECT_TRUE(manager.has_runnable_job());
}
