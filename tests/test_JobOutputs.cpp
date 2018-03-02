#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Output.h>
#include <workflow/Step.h>
#include <workflow/Workflow.h>
#include <Entity.h>
#include <errors.h>
#include "mocks/MockQueue.h"
#include "mocks/MockQueueFactory.h"
#include <queues/JobOutputs.h>


using ::testing::Return;

TEST(JobOutputsTest, test_get_status_two_empty_outputs) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {"output1", "output2"});
    auto outputs = step->get_outputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *output1 = new mimo::MockQueue();
    auto *output2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(output1))
        .WillOnce(Return(output2));

    EXPECT_CALL(*output1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*output2, can_push())
        .WillRepeatedly(Return(true));

    mimo::JobOutputs job_outputs(outputs, factory);
    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    outputs["output1"]->sync_group = 1;
    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::CAN_PUSH);
}

TEST(JobOutputsTest, test_get_status_one_full_one_empty_output) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {"output1", "output2"});
    auto outputs = step->get_outputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *output1 = new mimo::MockQueue();
    auto *output2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(output1))
        .WillOnce(Return(output2));

    bool output1_return_value = outputs.begin()->first == "output1";
    EXPECT_CALL(*output1, can_push())
        .WillRepeatedly(Return(output1_return_value));
    EXPECT_CALL(*output2, can_push())
        .WillRepeatedly(Return(!output1_return_value));

    mimo::JobOutputs job_outputs(outputs, factory);
    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    outputs["output1"]->sync_group = 1;
    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
}

TEST(JobOutputsTest, test_get_status_two_full_outputs) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {"output1", "output2"});
    auto outputs = step->get_outputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *output1 = new mimo::MockQueue();
    auto *output2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(output1))
        .WillOnce(Return(output2));

    EXPECT_CALL(*output1, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*output2, can_push())
        .WillRepeatedly(Return(false));

    mimo::JobOutputs job_outputs(outputs, factory);
    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    outputs["output1"]->sync_group = 1;
    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
}
