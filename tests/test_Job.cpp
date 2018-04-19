#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/Job.h"

#include <workflow/Workflow.h>
#include "mimo/Entity.h"
#include "mimo/Step.h"
#include "mocks/MockQueueBundle.h"
#include "mocks/MockQueueBundleFactory.h"
#include "mocks/MockStep.h"


using ::testing::_;
using ::testing::Return;

TEST(JobTest, test_job_without_input_or_output) {
    auto workflow = workflow::Workflow();
    auto step_id = workflow.add_step("step", {}, {});
    auto step = std::make_unique<mimo::MockStep>();
    auto factory = std::make_shared<mimo::MockQueueBundleFactory>();
    auto inputs = std::make_shared<mimo::MockQueueBundle>();
    auto outputs = std::make_shared<mimo::MockQueueBundle>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_shared(step_id->get_inputs()))
        .WillOnce(Return(inputs));
    EXPECT_CALL(*factory, make_shared(step_id->get_outputs()))
        .WillOnce(Return(outputs));

    mimo::Job job(step_id, std::move(step), factory);
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_FALSE(job.can_run());
}

TEST(JobTest, test_job_without_input) {
    auto workflow = workflow::Workflow();
    auto step_id = workflow.add_step("step", {}, {"output"});
    auto step = std::make_unique<mimo::MockStep>();
    auto factory = std::make_shared<mimo::MockQueueBundleFactory>();
    auto inputs = std::make_shared<mimo::MockQueueBundle>();
    auto outputs = std::make_shared<mimo::MockQueueBundle>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_shared(step_id->get_inputs()))
        .WillOnce(Return(inputs));
    EXPECT_CALL(*factory, make_shared(step_id->get_outputs()))
        .WillOnce(Return(outputs));

    mimo::Job job(step_id, std::move(step), factory);
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_FALSE(job.can_run());
}

TEST(JobTest, test_job_without_output) {
    auto workflow = workflow::Workflow();
    auto step_id = workflow.add_step("step", {"input"}, {});
    auto step = std::make_unique<mimo::MockStep>();
    auto factory = std::make_shared<mimo::MockQueueBundleFactory>();
    auto inputs = std::make_shared<mimo::MockQueueBundle>();
    auto outputs = std::make_shared<mimo::MockQueueBundle>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_shared(step_id->get_inputs()))
        .WillOnce(Return(inputs));
    EXPECT_CALL(*factory, make_shared(step_id->get_outputs()))
        .WillOnce(Return(outputs));

    mimo::Job job(step_id, std::move(step), factory);
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_TRUE(job.can_run());
    job.run();
    EXPECT_FALSE(job.can_run());
}

TEST(JobTest, test_job_completes_and_closes) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_unique<mimo::MockStep>();
    auto factory = std::make_shared<mimo::MockQueueBundleFactory>();
    auto job_inputs = std::make_shared<mimo::MockQueueBundle>();
    auto job_outputs = std::make_shared<mimo::MockQueueBundle>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(true));
    EXPECT_CALL(*factory, make_shared(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*factory, make_shared(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    //EXPECT_CALL(*job_outputs, end_run());

    mimo::Job job(identifier, std::move(step), factory);
    job.run();
}

TEST(JobTest, test_job_not_complete) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_unique<mimo::MockStep>();
    auto factory = std::make_shared<mimo::MockQueueBundleFactory>();
    auto job_inputs = std::make_shared<mimo::MockQueueBundle>();
    auto job_outputs = std::make_shared<mimo::MockQueueBundle>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false));
    EXPECT_CALL(*factory, make_shared(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*factory, make_shared(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    //EXPECT_CALL(*job_outputs, end_run());

    mimo::Job job(identifier, std::move(step), factory);
    job.run();
}

TEST(JobTest, test_job_complete_but_input_open) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_unique<mimo::MockStep>();
    auto factory = std::make_shared<mimo::MockQueueBundleFactory>();
    auto job_inputs = std::make_shared<mimo::MockQueueBundle>();
    auto job_outputs = std::make_shared<mimo::MockQueueBundle>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false));
    EXPECT_CALL(*factory, make_shared(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*factory, make_shared(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    //EXPECT_CALL(*job_outputs, end_run());

    mimo::Job job(identifier, std::move(step), factory);
    job.run();
}
