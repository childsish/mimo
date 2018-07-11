#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Output.h>
#include <workflow/Workflow.h>
#include "mocks/MockFactory.h"
#include "mocks/MockQueue.h"
#include "../src/errors.h"
#include "../src/Factory.h"
#include "../src/queues/IQueue.h"

#include "../src/queues/QueueBundle.h"


using ::testing::Return;

MOCK_FACTORY0(MockQueue, mimo::IQueue)

TEST(QueueBundleTest, no_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {}, {});
    auto input_ids = std::static_pointer_cast<mimo::ConnectionMap>(step->get_inputs());
    auto factory = std::make_shared<MockQueueFactory>();

    EXPECT_CALL(*factory, make_raw())
        .Times(0);

    mimo::QueueBundle bundle(input_ids, factory);
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_push_status(""), mimo::QueueBundle::PushStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_pop_status(), mimo::QueueBundle::PopStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_pop_status(""), mimo::QueueBundle::PopStatus::NO_QUEUE);

    EXPECT_THROW(bundle.push("", std::make_shared<mimo::Entity>()), mimo::QueueError);
    EXPECT_THROW(bundle.peek(""), mimo::QueueError);
    EXPECT_THROW(bundle.pop(""), mimo::QueueError);
}

TEST(QueueBundleTest, one_empty_queue) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input"}, {});
    auto input_ids = std::static_pointer_cast<mimo::ConnectionMap >(step->get_inputs());
    auto factory = std::make_shared<MockQueueFactory>();
    auto *input = new mimo::MockQueue();
    auto entity = std::make_shared<mimo::Entity>();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input));
    EXPECT_CALL(*input, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input, push(entity))
        .Times(1);

    mimo::QueueBundle bundle(input_ids, factory);
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status(""), mimo::QueueBundle::PushStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_push_status("input"), mimo::QueueBundle::PushStatus::CAN_PUSH);

    EXPECT_EQ(bundle.get_pop_status(), mimo::QueueBundle::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(bundle.get_pop_status(""), mimo::QueueBundle::PopStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_pop_status("input"), mimo::QueueBundle::PopStatus::QUEUE_EMPTY);

    bundle.push("input", entity);
    EXPECT_THROW(bundle.peek("input"), mimo::QueueError);
    EXPECT_THROW(bundle.pop("input"), mimo::QueueError);
}

TEST(QueueBundleTest, one_full_queue) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input"}, {});
    auto input_ids = std::static_pointer_cast<mimo::ConnectionMap >(step->get_inputs());
    auto factory = std::make_shared<MockQueueFactory>();
    auto *input = new mimo::MockQueue();
    auto entity = std::make_shared<mimo::Entity>();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input));
    EXPECT_CALL(*input, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input, peek())
        .Times(1);
    EXPECT_CALL(*input, pop())
        .Times(1);

    mimo::QueueBundle bundle(input_ids, factory);
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(bundle.get_push_status(""), mimo::QueueBundle::PushStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_push_status("input"), mimo::QueueBundle::PushStatus::QUEUE_FULL);

    EXPECT_EQ(bundle.get_pop_status(), mimo::QueueBundle::PopStatus::CAN_POP);
    EXPECT_EQ(bundle.get_pop_status(""), mimo::QueueBundle::PopStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_pop_status("input"), mimo::QueueBundle::PopStatus::CAN_POP);

    EXPECT_THROW(bundle.push("input", entity), mimo::QueueError);
    bundle.peek("input");
    bundle.pop("input");
}

TEST(QueueBundleTest, test_two_empty_aync_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input1", "input2"}, {});
    auto input_ids = std::static_pointer_cast<mimo::ConnectionMap >(step->get_inputs());
    auto factory = std::make_shared<MockQueueFactory>();
    auto *input1 = new mimo::MockQueue();
    auto *input2 = new mimo::MockQueue();
    auto entity = std::make_shared<mimo::Entity>();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input1))
        .WillOnce(Return(input2));
    EXPECT_CALL(*input1, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input1, push(entity))
        .Times(1);
    EXPECT_CALL(*input2, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input2, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input2, push(entity))
        .Times(1);

    mimo::QueueBundle bundle(input_ids, factory);
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status(""), mimo::QueueBundle::PushStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_push_status("input1"), mimo::QueueBundle::PushStatus::CAN_PUSH);
    EXPECT_EQ(bundle.get_push_status("input2"), mimo::QueueBundle::PushStatus::CAN_PUSH);

    EXPECT_EQ(bundle.get_pop_status(), mimo::QueueBundle::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(bundle.get_pop_status(""), mimo::QueueBundle::PopStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_pop_status("input1"), mimo::QueueBundle::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(bundle.get_pop_status("input2"), mimo::QueueBundle::PopStatus::QUEUE_EMPTY);

    bundle.push("input1", entity);
    EXPECT_THROW(bundle.peek("input1"), mimo::QueueError);
    EXPECT_THROW(bundle.pop("input1"), mimo::QueueError);
    bundle.push("input2", entity);
    EXPECT_THROW(bundle.peek("input2"), mimo::QueueError);
    EXPECT_THROW(bundle.pop("input2"), mimo::QueueError);
}

TEST(QueueBundleTest, test_two_full_aync_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input1", "input2"}, {});
    auto input_ids = std::static_pointer_cast<mimo::ConnectionMap >(step->get_inputs());
    auto factory = std::make_shared<MockQueueFactory>();
    auto *input1 = new mimo::MockQueue();
    auto *input2 = new mimo::MockQueue();
    auto entity = std::make_shared<mimo::Entity>();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input1))
        .WillOnce(Return(input2));
    EXPECT_CALL(*input1, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input1, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input1, peek())
        .Times(1);
    EXPECT_CALL(*input1, pop())
        .Times(1);
    EXPECT_CALL(*input2, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input2, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input2, peek())
        .Times(1);
    EXPECT_CALL(*input2, pop())
        .Times(1);

    mimo::QueueBundle bundle(input_ids, factory);
    EXPECT_EQ(bundle.get_push_status(), mimo::QueueBundle::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(bundle.get_push_status(""), mimo::QueueBundle::PushStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_push_status("input1"), mimo::QueueBundle::PushStatus::QUEUE_FULL);
    EXPECT_EQ(bundle.get_push_status("input2"), mimo::QueueBundle::PushStatus::QUEUE_FULL);

    EXPECT_EQ(bundle.get_pop_status(), mimo::QueueBundle::PopStatus::CAN_POP);
    EXPECT_EQ(bundle.get_pop_status(""), mimo::QueueBundle::PopStatus::NO_QUEUE);
    EXPECT_EQ(bundle.get_pop_status("input1"), mimo::QueueBundle::PopStatus::CAN_POP);
    EXPECT_EQ(bundle.get_pop_status("input2"), mimo::QueueBundle::PopStatus::CAN_POP);

    EXPECT_THROW(bundle.push("input1", entity), mimo::QueueError);
    bundle.peek("input1");
    bundle.pop("input1");
    EXPECT_THROW(bundle.push("input2", entity), mimo::QueueError);
    bundle.peek("input2");
    bundle.pop("input2");
}

TEST(QueueBundleTest, test_one_full_one_empty_async_queues) {
    workflow::Workflow workflow;
    auto step = workflow.add_step("step", {"input1", "input2"}, {});
    auto input_ids = std::static_pointer_cast<mimo::ConnectionMap >(step->get_inputs());
    auto factory = std::make_shared<MockQueueFactory>();
    auto *input1 = new mimo::MockQueue();
    auto *input2 = new mimo::MockQueue();
    auto entity = std::make_shared<mimo::Entity>();

    EXPECT_CALL(*factory, make_raw())
        .WillOnce(Return(input1))
        .WillOnce(Return(input2));
    EXPECT_CALL(*input1, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input1, can_push())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input1, push(entity))
        .Times(1);
    EXPECT_CALL(*input2, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*input2, can_push())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*input2, peek())
        .Times(1);
    EXPECT_CALL(*input2, pop())
        .Times(1);

    mimo::QueueBundle bundle(input_ids, factory);
    std::unordered_set<int> actual;
    actual.insert(static_cast<int>(bundle.get_push_status("input1")));
    actual.insert(static_cast<int>(bundle.get_push_status("input2")));
    std::unordered_set<int> expected;
    expected.insert(static_cast<int>(mimo::QueueBundle::PushStatus::CAN_PUSH));
    expected.insert(static_cast<int>(mimo::QueueBundle::PushStatus::QUEUE_FULL));
    EXPECT_EQ(actual, expected);
    actual.clear();
    expected.clear();

    EXPECT_EQ(bundle.get_pop_status(), mimo::QueueBundle::PopStatus::CAN_POP);
    EXPECT_EQ(bundle.get_pop_status(""), mimo::QueueBundle::PopStatus::NO_QUEUE);
    actual.insert(static_cast<int>(bundle.get_pop_status("input1")));
    actual.insert(static_cast<int>(bundle.get_pop_status("input2")));
    expected.insert(static_cast<int>(mimo::QueueBundle::PopStatus::QUEUE_EMPTY));
    expected.insert(static_cast<int>(mimo::QueueBundle::PopStatus::CAN_POP));

    if (bundle.get_push_status("input1") == mimo::QueueBundle::PushStatus::CAN_PUSH) {
        bundle.push("input1", entity);
        EXPECT_THROW(bundle.peek("input1"), mimo::QueueError);
        EXPECT_THROW(bundle.pop("input1"), mimo::QueueError);
        EXPECT_THROW(bundle.push("input2", entity), mimo::QueueError);
        bundle.peek("input2");
        bundle.pop("input2");
    }
    else {
        EXPECT_THROW(bundle.push("input1", entity), mimo::QueueError);
        bundle.peek("input1");
        bundle.pop("input1");
        bundle.push("input2", entity);
        EXPECT_THROW(bundle.peek("input2"), mimo::QueueError);
        EXPECT_THROW(bundle.pop("input2"), mimo::QueueError);
    }
}
