#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Input.h>
#include <workflow/Step.h>
#include <workflow/Workflow.h>
#include <Entity.h>
#include <errors.h>
#include "mocks/MockQueue.h"
#include "mocks/MockQueueFactory.h"
#include <queues/JobInputs.h>


using ::testing::Return;

TEST(JobInputsTest, test_get_status_two_full_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input1", "input2"}, {});
    auto inputs = step->get_inputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *input1 = new mimo::MockQueue();
    auto *input2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input1))
        .WillOnce(Return(input2));

    EXPECT_CALL(*input1, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input2, can_pop())
        .WillRepeatedly(Return(true));

    mimo::JobInputs job_inputs(inputs, factory);
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("input1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("input2"), mimo::JobInputs::PopStatus::CAN_POP);
    inputs["input1"]->sync_group = 1;
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("input1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("input2"), mimo::JobInputs::PopStatus::CAN_POP);
}

TEST(JobInputsTest, test_get_status_one_full_one_empty_queue) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input1", "input2"}, {});
    auto inputs = step->get_inputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *input1 = new mimo::MockQueue();
    auto *input2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input1))
        .WillOnce(Return(input2));

    bool input1_return_value = inputs.begin()->first == "input1";
    EXPECT_CALL(*input1, can_pop())
        .WillRepeatedly(Return(input1_return_value));
    EXPECT_CALL(*input2, can_pop())
        .WillRepeatedly(Return(!input1_return_value));

    mimo::JobInputs job_inputs(inputs, factory);
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("input1"), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("input2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    inputs["input1"]->sync_group = 1;
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("input1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("input2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
}

TEST(JobInputsTest, test_get_status_two_empty_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input1", "input2"}, {});
    auto inputs = step->get_inputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *input1 = new mimo::MockQueue();
    auto *input2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input1))
        .WillOnce(Return(input2));

    EXPECT_CALL(*input1, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input2, can_pop())
        .WillRepeatedly(Return(false));

    mimo::JobInputs job_inputs(inputs, factory);
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("input1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("input2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    inputs["input1"]->sync_group = 1;
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("input1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("input2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
}
