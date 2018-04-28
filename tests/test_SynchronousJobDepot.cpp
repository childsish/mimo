#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include <mimo/Entity.h>
#include "mocks/MockJob.h"
#include "mocks/MockJobFactory.h"
#include "mocks/MockStep.h"
#include "mocks/MockQueueBundle.h"
#include "../src/job_depots/SynchronousJobDepot.h"


using ::testing::Return;

TEST(SynchronousJobManagerTest, test_empty_job_not_runnable) {
    workflow::Workflow workflow;

    auto job_proxy = new mimo::MockJob();
    EXPECT_CALL(*job_proxy, can_run())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*job_proxy, get_job_id())
        .WillRepeatedly(Return(0));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    auto identifier = workflow.add_step("step1", {}, {});
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy));

    mimo::SynchronousJobDepot depot(2, identifier, step, factory);

    EXPECT_FALSE(depot.has_runnable_job());
}

TEST(SynchronousJobManagerTest, test_capacity) {
    workflow::Workflow workflow;

    auto identifier = workflow.add_step("step1", {}, {});
    auto job_proxy0 = new mimo::MockJob();
    EXPECT_CALL(*job_proxy0, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy0, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy0, get_job_id())
        .WillRepeatedly(Return(0));

    auto job_proxy1 = new mimo::MockJob();
    EXPECT_CALL(*job_proxy1, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy1, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy1, get_job_id())
        .WillRepeatedly(Return(1));

    auto job_proxy2 = new mimo::MockJob();
    EXPECT_CALL(*job_proxy2, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy2, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy2, get_job_id())
        .WillRepeatedly(Return(2));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy0))
        .WillOnce(Return(job_proxy1))
        .WillOnce(Return(job_proxy2));

    mimo::SynchronousJobDepot depot(2, identifier, step, factory);

    EXPECT_TRUE(depot.has_runnable_job());
    auto job1 = depot.get_runnable_job();
    EXPECT_TRUE(depot.has_runnable_job());
    auto job2 = depot.get_runnable_job();
    EXPECT_FALSE(depot.has_runnable_job());
}

TEST(SynchronousJobManagerTest, test_return_wrong_job) {
    workflow::Workflow workflow;

    auto identifier = workflow.add_step("step1", {}, {});
    auto job_proxy0 = new mimo::MockJob();
    EXPECT_CALL(*job_proxy0, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy0, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy0, get_job_id())
        .WillRepeatedly(Return(0));

    auto job_proxy1 = new mimo::MockJob();
    EXPECT_CALL(*job_proxy1, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy1, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy1, get_job_id())
        .WillRepeatedly(Return(1));

    auto factory = std::make_shared<mimo::MockJobFactory>();
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy0))
        .WillOnce(Return(job_proxy1));

    mimo::SynchronousJobDepot depot(1, identifier, step, factory);

    auto wrong_job = std::make_shared<mimo::MockJob>();
    auto wrong_identifier = workflow.add_step("step2", {}, {});
    EXPECT_CALL(*wrong_job, get_step_id())
        .WillRepeatedly(Return(wrong_identifier));
    EXPECT_THROW(depot.return_job(wrong_job), std::runtime_error);

    auto job = depot.get_runnable_job();
    EXPECT_THROW(depot.return_job(wrong_job), std::runtime_error);
}

TEST(SynchronousJobManagerTest, test_inputs_always_pushed_to_latest_job) {
    workflow::Workflow workflow;
    auto entity = std::make_shared<mimo::Entity>();

    auto identifier = workflow.add_step("step1", {"input"}, {});
    auto job_proxy0 = new mimo::MockJob();
    auto inputs0 = std::make_shared<mimo::MockQueueBundle>();
    EXPECT_CALL(*job_proxy0, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy0, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy0, get_job_id())
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*job_proxy0, get_inputs())
        .WillRepeatedly(Return(inputs0));
    EXPECT_CALL(*inputs0, push("input", entity))
        .Times(1);

    auto job_proxy1 = new mimo::MockJob();
    auto inputs1 = std::make_shared<mimo::MockQueueBundle>();
    EXPECT_CALL(*job_proxy1, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*job_proxy1, get_step_id())
        .WillRepeatedly(Return(identifier));
    EXPECT_CALL(*job_proxy1, get_job_id())
        .WillRepeatedly(Return(1));
    EXPECT_CALL(*job_proxy1, get_inputs())
        .WillRepeatedly(Return(inputs1));
    EXPECT_CALL(*inputs1, push("input", entity))
        .Times(2);

    auto factory = std::make_shared<mimo::MockJobFactory>();
    std::shared_ptr<mimo::Step> step = std::make_shared<mimo::MockStep>();
    EXPECT_CALL(*factory, make_raw(identifier, step))
        .WillOnce(Return(job_proxy0))
        .WillOnce(Return(job_proxy1));

    mimo::SynchronousJobDepot depot(2, identifier, step, factory);

    depot.add_entity(identifier->get_input("input"), entity);
    auto job0 = depot.get_runnable_job();

    depot.add_entity(identifier->get_input("input"), entity);
    depot.return_job(job0);
    depot.add_entity(identifier->get_input("input"), entity);
}
