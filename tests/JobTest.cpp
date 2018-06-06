#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <workflow/Workflow.h>
#include "mimo/Entity.h"
#include "mimo/Step.h"
#include "mocks/MockFactory.h"
#include "mocks/MockQueue.h"
#include "mocks/MockQueueBundle.h"
#include "mocks/MockStep.h"

#include "../src/Job.h"


using ::testing::_;
using ::testing::Return;

MOCK_FACTORY1(MockQueueBundle, mimo::IQueueBundle, std::shared_ptr<workflow::ConnectionMap>)

TEST(JobTest, test_job_without_input_or_output) {
    auto workflow = workflow::Workflow();
    auto step_id = workflow.add_step("step", {}, {});
    auto step = std::make_shared<mimo::MockStep>();
    auto factory = std::make_shared<MockQueueBundleFactory>();
    auto inputs = new mimo::MockQueueBundle();
    auto outputs = new mimo::MockQueueBundle();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_raw(step_id->get_inputs()))
        .WillOnce(Return(inputs));
    EXPECT_CALL(*factory, make_raw(step_id->get_outputs()))
        .WillOnce(Return(outputs));

    mimo::Job job(0, step_id, step, factory);
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_FALSE(job.can_run());
}

TEST(JobTest, test_job_without_input) {
    auto workflow = workflow::Workflow();
    auto step_id = workflow.add_step("step", {}, {"output"});
    auto step = std::make_shared<mimo::MockStep>();
    auto factory = std::make_shared<MockQueueBundleFactory>();
    auto inputs = new mimo::MockQueueBundle();
    auto outputs = new mimo::MockQueueBundle();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_raw(step_id->get_inputs()))
        .WillOnce(Return(inputs));
    EXPECT_CALL(*factory, make_raw(step_id->get_outputs()))
        .WillOnce(Return(outputs));

    mimo::Job job(0, step_id, step, factory);
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_FALSE(job.can_run());
}

TEST(JobTest, test_job_without_output) {
    auto workflow = workflow::Workflow();
    auto step_id = workflow.add_step("step", {"input"}, {});
    auto step = std::make_shared<mimo::MockStep>();
    auto factory = std::make_shared<MockQueueBundleFactory>();
    auto inputs = new mimo::MockQueueBundle();
    auto outputs = new mimo::MockQueueBundle();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_raw(step_id->get_inputs()))
        .WillOnce(Return(inputs));
    EXPECT_CALL(*factory, make_raw(step_id->get_outputs()))
        .WillOnce(Return(outputs));

    mimo::Job job(0, step_id, step, factory);
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_FALSE(job.can_run());
}

TEST(JobTest, test_job_completes_and_closes) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_shared<mimo::MockStep>();
    auto factory = std::make_shared<MockQueueBundleFactory>();
    auto job_inputs = new mimo::MockQueueBundle();
    auto job_outputs = new mimo::MockQueueBundle();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_raw(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*factory, make_raw(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    mimo::Job job(0, identifier, std::move(step), factory);
    job.run();
}

TEST(JobTest, test_job_not_complete) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_shared<mimo::MockStep>();
    auto factory = std::make_shared<MockQueueBundleFactory>();
    auto job_inputs = new mimo::MockQueueBundle();
    auto job_outputs = new mimo::MockQueueBundle();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false));
    EXPECT_CALL(*factory, make_raw(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*factory, make_raw(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    mimo::Job job(0, identifier, std::move(step), factory);
    job.run();
}

TEST(JobTest, test_job_complete_but_input_open) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_shared<mimo::MockStep>();
    auto factory = std::make_shared<MockQueueBundleFactory>();
    auto job_inputs = new mimo::MockQueueBundle();
    auto job_outputs = new mimo::MockQueueBundle();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false));
    EXPECT_CALL(*factory, make_raw(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*factory, make_raw(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    mimo::Job job(0, identifier, std::move(step), factory);
    job.run();
}
