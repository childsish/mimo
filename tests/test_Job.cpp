#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Job.h"

#include "Entity.h"
#include "Step.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "mocks/MockQueueBundle.h"
#include "mocks/MockQueueBundleFactory.h"
#include "mocks/MockStep.h"


using ::testing::_;
using ::testing::Return;

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
