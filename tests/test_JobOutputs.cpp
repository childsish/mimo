#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "queues/Outputs.h"

#include <queue>
#include "errors.h"
#include "Entity.h"
#include "IQueueFactory.h"
#include "queues/IQueue.h"


using ::testing::Return;

class MockQueue : public mimo::IQueue {
public:
    MOCK_METHOD0(peek, std::shared_ptr<mimo::Entity>());
    MOCK_METHOD0(pop, std::shared_ptr<mimo::Entity>());
    MOCK_METHOD1(push, void(std::shared_ptr<mimo::Entity> entity));
    MOCK_CONST_METHOD0(can_pop, bool());
    MOCK_CONST_METHOD0(can_push, bool());
    MOCK_CONST_METHOD0(is_closed, bool());
    MOCK_CONST_METHOD0(is_empty, bool());
    MOCK_CONST_METHOD0(is_full, bool());
};

class MockFactory : public mimo::IQueueFactory {
public:
    virtual std::unique_ptr<mimo::IQueue> make() {
        return std::unique_ptr<mimo::IQueue>(this->make_proxy());
    }

    MOCK_METHOD0(make_proxy, mimo::IQueue*());
};

TEST(OutputsTest, test_asynced_queues) {
    auto queue1 = new MockQueue();
    EXPECT_CALL(*queue1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue1, is_full())
            .WillRepeatedly(Return(false));

    auto queue2 = new MockQueue();
    EXPECT_CALL(*queue2, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*queue2, is_full())
        .WillRepeatedly(Return(true));

    MockFactory factory;
    EXPECT_CALL(factory, make_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    mimo::JobOutputs outputs(factory, {"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
}

TEST(OutputsTest, test_synced_queues) {
    auto queue1 = new MockQueue();
    EXPECT_CALL(*queue1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue1, is_full())
        .WillRepeatedly(Return(false));

    auto queue2 = new MockQueue();
    EXPECT_CALL(*queue2, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, is_full())
        .WillRepeatedly(Return(false));

    MockFactory factory;
    EXPECT_CALL(factory, make_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    mimo::JobOutputs outputs(factory, {"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);
}
