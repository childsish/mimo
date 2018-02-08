#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "queues/JobInputs.h"

#include "Entity.h"
#include "errors.h"
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

TEST(InputsTest, test_asynced_queues) {
    auto queue1 = std::make_unique<MockQueue>();
    EXPECT_CALL(*queue1, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*queue1, is_empty())
        .WillRepeatedly(Return(true));

    auto queue2 = std::make_unique<MockQueue>();
    EXPECT_CALL(*queue2, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, is_empty())
        .WillRepeatedly(Return(false));

    mimo::JobInputs inputs;
    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));

    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
}

TEST(InputsTest, test_synced_queues) {
    auto queue1 = std::make_unique<MockQueue>();
    EXPECT_CALL(*queue1, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue1, is_empty())
        .WillRepeatedly(Return(false));

    auto queue2 = std::make_unique<MockQueue>();
    EXPECT_CALL(*queue2, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, is_empty())
        .WillRepeatedly(Return(false));

    mimo::JobInputs inputs;
    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));


    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);
}