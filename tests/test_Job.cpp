#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Job.h"

#include "Entity.h"
#include "Step.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "mocks/MockJobInputs.h"
#include "mocks/MockJobInputsFactory.h"
#include "mocks/MockJobOutputs.h"
#include "mocks/MockJobOutputsFactory.h"
#include "mocks/MockStep.h"


using ::testing::_;
using ::testing::Return;

TEST(JobTest, test_job_completes_and_closes) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_unique<mimo::MockStep>();
    auto input_factory = std::make_shared<mimo::MockJobInputsFactory>();
    auto output_factory = std::make_shared<mimo::MockJobOutputsFactory>();
    auto *job_inputs = new mimo::MockJobInputs();
    auto *job_outputs = new mimo::MockJobOutputs();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(true));
    EXPECT_CALL(*input_factory, make_raw(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*output_factory, make_raw(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    EXPECT_CALL(*job_outputs, end_run());

    mimo::Job job(identifier, std::move(step), input_factory, output_factory);
    job.run();
}

TEST(JobTest, test_job_not_complete) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_unique<mimo::MockStep>();
    auto input_factory = std::make_shared<mimo::MockJobInputsFactory>();
    auto output_factory = std::make_shared<mimo::MockJobOutputsFactory>();
    auto *job_inputs = new mimo::MockJobInputs();
    auto *job_outputs = new mimo::MockJobOutputs();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false));
    EXPECT_CALL(*input_factory, make_raw(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*output_factory, make_raw(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    EXPECT_CALL(*job_outputs, end_run());

    mimo::Job job(identifier, std::move(step), input_factory, output_factory);
    job.run();
}

TEST(JobTest, test_job_complete_but_input_open) {
    auto identifier = std::make_shared<workflow::Step>("step");
    auto step = std::make_unique<mimo::MockStep>();
    auto input_factory = std::make_shared<mimo::MockJobInputsFactory>();
    auto output_factory = std::make_shared<mimo::MockJobOutputsFactory>();
    auto *job_inputs = new mimo::MockJobInputs();
    auto *job_outputs = new mimo::MockJobOutputs();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(false));
    EXPECT_CALL(*input_factory, make_raw(identifier->get_inputs()))
        .WillOnce(Return(job_inputs));
    EXPECT_CALL(*output_factory, make_raw(identifier->get_outputs()))
        .WillOnce(Return(job_outputs));

    EXPECT_CALL(*job_outputs, end_run());

    mimo::Job job(identifier, std::move(step), input_factory, output_factory);
    job.run();
}
