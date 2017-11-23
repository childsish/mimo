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
    mimo::QueueChannel channel(1);

    auto queue = std::make_unique<mimo::Queue>(0);
    EXPECT_THROW(channel.push(std::move(queue)), std::runtime_error);

    queue = std::make_unique<mimo::Queue>(0);
    EXPECT_TRUE(channel.reserve(queue));
    EXPECT_FALSE(channel.reserve(queue));
    channel.push(std::move(queue));
    EXPECT_TRUE(channel.can_pop());

    queue = std::make_unique<mimo::Queue>(0);
    EXPECT_FALSE(channel.reserve(queue));
}

TEST(QueueChannelTest, test_pop) {
    mimo::QueueChannel channel;

    auto queue = std::make_unique<mimo::Queue>(2);
    EXPECT_TRUE(channel.reserve(queue));
    channel.push(std::move(queue));
    EXPECT_FALSE(channel.can_pop());

    queue = std::make_unique<mimo::Queue>(0);
    EXPECT_TRUE(channel.reserve(queue));
    channel.push(std::move(queue));
    EXPECT_TRUE(channel.can_pop());
    channel.pop();

    queue = std::make_unique<mimo::Queue>(1);
    EXPECT_TRUE(channel.reserve(queue));
    channel.push(std::move(queue));
    EXPECT_TRUE(channel.can_pop());
    channel.pop();
    EXPECT_TRUE(channel.can_pop());
    channel.pop();
}

TEST(QueueChannelTest, test_popping_in_order) {
    mimo::QueueChannel channel;
    auto queue0 = std::make_unique<mimo::Queue>(0);
    auto queue1 = std::make_unique<mimo::Queue>(1);
    auto queue2 = std::make_unique<mimo::Queue>(2);

    channel.reserve(queue2);
    channel.push(std::move(queue2));
    channel.reserve(queue1);
    channel.push(std::move(queue1));
    channel.reserve(queue0);
    channel.push(std::move(queue0));

    EXPECT_EQ(channel.pop()->index, 0);
    EXPECT_EQ(channel.pop()->index, 1);
    EXPECT_EQ(channel.pop()->index, 2);
}

TEST(QueueChannelTest, test_reserve_last_position_when_current_index_present) {
    mimo::QueueChannel channel(3);
    auto queue0 = std::make_unique<mimo::Queue>(0);
    auto queue1 = std::make_unique<mimo::Queue>(1);
    auto queue2 = std::make_unique<mimo::Queue>(2);

    channel.reserve(queue0);
    channel.push(std::move(queue0));
    channel.reserve(queue1);
    channel.push(std::move(queue1));

    EXPECT_TRUE(channel.reserve(queue2));
}


TEST(QueueChannelTest, test_reserve_last_position_when_current_index_absent) {
    mimo::QueueChannel channel(3);
    auto queue0 = std::make_unique<mimo::Queue>(0);
    auto queue1 = std::make_unique<mimo::Queue>(1);
    auto queue2 = std::make_unique<mimo::Queue>(2);
    auto queue3 = std::make_unique<mimo::Queue>(3);

    channel.reserve(queue2);
    channel.push(std::move(queue2));
    channel.reserve(queue1);
    channel.push(std::move(queue1));

    EXPECT_FALSE(channel.reserve(queue3));
    EXPECT_TRUE(channel.reserve(queue0));
}
