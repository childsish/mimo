#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Job.h"

#include <workflow/Workflow.h>
#include "Entity.h"
#include "Step.h"
#include "queues/IQueue.h"
#include "queues/IJobInputs.h"
#include "queues/IJobOutputs.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include "mocks/MockJobInputs.h"
#include "mocks/MockJobOutputs.h"
#include "mocks/MockStep.h"


using ::testing::_;
using ::testing::Return;

TEST(JobTest, test_job_completes_and_closes) {
    auto workflow = workflow::Workflow();
    auto identifier = workflow.add_step("step", {}, {});
    auto step = std::make_unique<MockStep>();
    auto inputs = std::make_unique<MockJobInputs>();
    auto outputs = std::make_unique<MockJobOutputs>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(true));
    EXPECT_CALL(*inputs, is_closed())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*outputs, close())
        .Times(1);
    EXPECT_CALL(*outputs, end_run())
        .Times(1);

    mimo::Job job(
        identifier,
        std::move(step),
        std::move(inputs),
        std::move(outputs)
    );
    job.run();
}

TEST(JobTest, test_job_not_complete) {
    auto workflow = workflow::Workflow();
    auto identifier = workflow.add_step("step", {}, {});
    auto step = std::make_unique<MockStep>();
    auto inputs = std::make_unique<MockJobInputs>();
    auto outputs = std::make_unique<MockJobOutputs>();

    EXPECT_CALL(*step, run(_, _))
            .WillOnce(Return(false));
    EXPECT_CALL(*inputs, is_closed())
            .WillRepeatedly(Return(true));
    EXPECT_CALL(*outputs, close())
            .Times(0);
    EXPECT_CALL(*outputs, end_run())
            .Times(1);

    mimo::Job job(
            identifier,
            std::move(step),
            std::move(inputs),
            std::move(outputs)
    );
    job.run();
}

TEST(JobTest, test_job_complete_but_input_open) {
    auto workflow = workflow::Workflow();
    auto identifier = workflow.add_step("step", {}, {});
    auto step = std::make_unique<MockStep>();
    auto inputs = std::make_unique<MockJobInputs>();
    auto outputs = std::make_unique<MockJobOutputs>();

    EXPECT_CALL(*step, run(_, _))
            .WillOnce(Return(true));
    EXPECT_CALL(*inputs, is_closed())
            .WillRepeatedly(Return(false));
    EXPECT_CALL(*outputs, close())
            .Times(0);
    EXPECT_CALL(*outputs, end_run())
            .Times(1);

    mimo::Job job(
            identifier,
            std::move(step),
            std::move(inputs),
            std::move(outputs)
    );
    job.run();
}
