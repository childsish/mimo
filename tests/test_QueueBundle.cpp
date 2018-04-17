#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Output.h>
#include <workflow/Workflow.h>
#include "mocks/MockQueue.h"
#include "mocks/MockQueueFactory.h"
#include "../src/queues/QueueBundle.h"


using ::testing::Return;

TEST(QueueBundleTest, test_get_status_two_empty_outputs) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {"output1", "output2"});
    auto outputs = step->get_outputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *output1 = new mimo::MockQueue();
    auto *output2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(output1))
        .WillOnce(Return(output2));

    EXPECT_CALL(*output1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*output2, can_push())
        .WillRepeatedly(Return(true));

    mimo::QueueBundle bundle(outputs, factory);
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status("output1"), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status("output2"), mimo::QueueBundle::PushStatus::CAN_PUSH);
    outputs["output1"]->sync_group = 1;
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status("output1"), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status("output2"), mimo::QueueBundle::PushStatus::CAN_PUSH);
}

TEST(QueueBundleTest, test_get_status_one_full_one_empty_output) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {"output1", "output2"});
    auto outputs = step->get_outputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *output1 = new mimo::MockQueue();
    auto *output2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(output1))
        .WillOnce(Return(output2));

    bool output1_return_value = outputs.begin()->first == "output1";
    EXPECT_CALL(*output1, can_push())
        .WillRepeatedly(Return(output1_return_value));
    EXPECT_CALL(*output2, can_push())
        .WillRepeatedly(Return(!output1_return_value));

    mimo::QueueBundle job_outputs(outputs, factory);
    EXPECT_EQ(job_outputs.get_push_status(), mimo::QueueBundle::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_push_status("output1"), mimo::QueueBundle::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_push_status("output2"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
    outputs["output1"]->sync_group = 1;
    EXPECT_EQ(job_outputs.get_push_status(), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_push_status("output1"), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_push_status("output2"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
}

TEST(QueueBundleTest, test_get_status_two_full_outputs) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {"output1", "output2"});
    auto outputs = step->get_outputs();
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *output1 = new mimo::MockQueue();
    auto *output2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(output1))
        .WillOnce(Return(output2));

    EXPECT_CALL(*output1, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*output2, can_push())
        .WillRepeatedly(Return(false));

    mimo::QueueBundle job_outputs(outputs, factory);
    EXPECT_EQ(job_outputs.get_push_status(), mimo::QueueBundle::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_push_status("output1"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_push_status("output2"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
    outputs["output1"]->sync_group = 1;
    EXPECT_EQ(job_outputs.get_push_status(), mimo::QueueBundle::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_push_status("output1"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_push_status("output2"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
}
