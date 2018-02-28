#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Output.h>
#include <workflow/Workflow.h>
#include <Entity.h>
#include <errors.h>
#include "mocks/MockQueue.h"
#include "mocks/MockQueueFactory.h"
#include <queues/JobOutputs.h>


using ::testing::Return;

TEST(OutputsTest, test_asynced_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step1", {"input1", "input2"}, {"output1", "output2"});
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *queue1 = new mimo::MockQueue();
    auto *queue2 = new mimo::MockQueue();

    step->get_output("output1")->sync_group = 0;
    step->get_output("output2")->sync_group = 1;

    EXPECT_CALL(*queue1, can_pop())
            .WillRepeatedly(Return(false));
    EXPECT_CALL(*queue1, is_empty())
            .WillRepeatedly(Return(true));

    EXPECT_CALL(*queue2, can_pop())
            .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, is_empty())
            .WillRepeatedly(Return(false));

    EXPECT_CALL(*factory, make_unique_proxy())
            .WillOnce(Return(queue1))
            .WillOnce(Return(queue2));

    mimo::JobOutputs job_outputs(step->get_outputs(), factory);

    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::CAN_PUSH);

    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output1"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    EXPECT_EQ(job_outputs.get_status("output2"), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
}

TEST(OutputsTest, test_synced_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step1", {"input1", "input2"}, {"output1", "output2"});
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *queue1 = new mimo::MockQueue();
    auto *queue2 = new mimo::MockQueue();

    EXPECT_CALL(*queue1, can_pop())
            .WillRepeatedly(Return(false));
    EXPECT_CALL(*queue1, is_empty())
            .WillRepeatedly(Return(true));

    EXPECT_CALL(*queue2, can_pop())
            .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, is_empty())
            .WillRepeatedly(Return(false));

    EXPECT_CALL(*factory, make_unique_proxy())
            .WillOnce(Return(queue1))
            .WillOnce(Return(queue2));

    mimo::JobOutputs job_outputs(step->get_outputs(), factory);

    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);

    EXPECT_EQ(job_outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(job_outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);
}
