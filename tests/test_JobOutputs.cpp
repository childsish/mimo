#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "errors.h"
#include "Entity.h"
#include "mocks/MockQueue.h"

#include "queues/JobOutputs.h"


using ::testing::Return;

TEST(JobOutputsTest, test_asynced_queues) {
    auto queue1 = new mimo::MockQueue();
    EXPECT_CALL(*queue1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue1, is_full())
        .WillRepeatedly(Return(false));

    auto queue2 = new mimo::MockQueue();
    EXPECT_CALL(*queue2, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*queue2, is_full())
        .WillRepeatedly(Return(true));

    auto factory = std::make_shared<mimo::MockQueueFactory>();
    EXPECT_CALL(*factory, make_shared_proxy())
        .Times(0);
    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    mimo::JobOutputs outputs({"queue1", "queue2"}, factory);
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
}

TEST(JobOutputsTest, test_synced_queues) {
    auto queue1 = new mimo::MockQueue();
    EXPECT_CALL(*queue1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue1, is_full())
        .WillRepeatedly(Return(false));

    auto queue2 = new mimo::MockQueue();
    EXPECT_CALL(*queue2, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, is_full())
        .WillRepeatedly(Return(false));

    auto factory = std::make_shared<mimo::MockQueueFactory>();
    EXPECT_CALL(*factory, make_shared_proxy())
        .Times(0);
    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    mimo::JobOutputs outputs({"queue1", "queue2"}, factory);
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);
}
