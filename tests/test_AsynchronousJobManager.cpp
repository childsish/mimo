#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include <job_managers/AsynchronousJobManager.h>
#include "mocks/MockJob.h"
#include "mocks/MockJobFactory.h"
#include "mocks/MockStep.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


using ::testing::Return;

TEST(AsynchronousJobManagerTest, test_empty_job_not_runnable) {
    workflow::Workflow workflow;

    auto job_proxy = new mimo::MockJob();
    EXPECT_CALL(*job_proxy, can_run())
        .WillRepeatedly(Return(false));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    auto identifier = workflow.add_step("step1", {}, {});
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy));

    mimo::AsynchronousJobManager manager(identifier, step, factory);

    EXPECT_FALSE(manager.has_runnable_job());
}

TEST(AsynchronousJobManagerTest, test_only_one_job_allowed) {
    workflow::Workflow workflow;

    auto identifier = workflow.add_step("step1", {}, {});
    auto job_proxy = new mimo::MockJob();
    EXPECT_CALL(*job_proxy, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy, get_step_id())
        .WillRepeatedly(Return(identifier));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy));

    mimo::AsynchronousJobManager manager(identifier, step, factory);

    EXPECT_TRUE(manager.has_runnable_job());
    auto job = manager.get_runnable_job();
    EXPECT_FALSE(manager.has_runnable_job());
    manager.return_complete_job(job);
    EXPECT_TRUE(manager.has_runnable_job());
}

TEST(AsynchronousJobManagerTest, test_return_wrong_job) {
    workflow::Workflow workflow;

    auto identifier = workflow.add_step("step1", {}, {});
    auto job_proxy = new mimo::MockJob();
    EXPECT_CALL(*job_proxy, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy, get_step_id())
        .WillRepeatedly(Return(identifier));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy));

    mimo::AsynchronousJobManager manager(identifier, step, factory);

    auto wrong_job = std::make_shared<mimo::MockJob>();
    auto wrong_identifier = workflow.add_step("step2", {}, {});
    EXPECT_CALL(*wrong_job, get_step_id())
        .WillRepeatedly(Return(wrong_identifier));
    EXPECT_THROW(manager.return_complete_job(wrong_job), std::runtime_error);

    auto job = manager.get_runnable_job();
    EXPECT_THROW(manager.return_complete_job(wrong_job), std::runtime_error);
}