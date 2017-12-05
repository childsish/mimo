/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "gtest/gtest.h"
#include "queues/QueueChannel.h"

TEST(QueueChannelTest, test_no_capacity) {
    EXPECT_THROW(mimo::QueueChannel(0), std::runtime_error);
}

TEST(QueueChannelTest, test_push) {
    mimo::QueueChannel channel(2);
    auto queue = std::make_unique<mimo::Queue>(1);

    EXPECT_EQ(channel.get_push_status(queue->run), mimo::QueueChannel::CAN_PUSH);
    EXPECT_NO_THROW(channel.push(std::move(queue)));

    queue = std::make_unique<mimo::Queue>(1);
    EXPECT_EQ(channel.get_push_status(queue->run), mimo::QueueChannel::PUSH_NEXT);

    queue = std::make_unique<mimo::Queue>(0);
    channel.push(std::move(queue));
    queue = std::make_unique<mimo::Queue>(1);
    EXPECT_EQ(channel.get_push_status(queue->run), mimo::QueueChannel::PUSH_FULL);
}

TEST(QueueChannelTest, test_pop) {
    mimo::QueueChannel channel;

    auto queue = std::make_unique<mimo::Queue>(2);
    channel.push(std::move(queue));
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_NOT_POP);

    queue = std::make_unique<mimo::Queue>(0);
    channel.push(std::move(queue));
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_POP);
    channel.pop();
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_NOT_POP);

    queue = std::make_unique<mimo::Queue>(1);
    channel.push(std::move(queue));
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_NOT_POP);
    queue = std::make_unique<mimo::Queue>(0);
    queue->end_run();
    channel.push(std::move(queue));
    channel.pop();
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_POP);

    EXPECT_EQ(channel.pop()->run, 1);
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_POP);
    EXPECT_EQ(channel.pop()->run, 2);
    EXPECT_EQ(channel.get_pop_status(), mimo::QueueChannel::CAN_NOT_POP);
}

TEST(QueueChannelTest, test_reserve_stays_till_queue_closed) {
    mimo::QueueChannel channel(2);

    auto queue0 = std::make_unique<mimo::Queue>(0);
    auto queue1 = std::make_unique<mimo::Queue>(1);
    channel.push(std::move(queue0));
    EXPECT_EQ(channel.get_push_status(queue1->run), mimo::QueueChannel::PUSH_NEXT);

    queue0 = std::make_unique<mimo::Queue>(0);
    queue0->end_run();
    channel.push(std::move(queue0));
    channel.pop();
    EXPECT_EQ(channel.get_push_status(queue1->run), mimo::QueueChannel::CAN_PUSH);
    EXPECT_NO_THROW(channel.push(std::move(queue1)));
}
