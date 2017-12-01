/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "gtest/gtest.h"
#include "queues/QueueChannel.h"

TEST(QueueChannelTest, test_no_capacity) {
    EXPECT_THROW(mimo::QueueChannel(0), std::runtime_error);
}

TEST(QueueChannelTest, test_reserve) {
    mimo::QueueChannel channel(2);

    EXPECT_TRUE(channel.can_reserve(0));
    EXPECT_TRUE(channel.can_reserve(1));
    EXPECT_TRUE(channel.can_reserve(2));

    channel.reserve(1);
    EXPECT_FALSE(channel.has_reservation(0));
    EXPECT_TRUE(channel.has_reservation(1));
    EXPECT_FALSE(channel.has_reservation(2));
    EXPECT_TRUE(channel.can_reserve(0));
    EXPECT_FALSE(channel.can_reserve(1));
    EXPECT_FALSE(channel.can_reserve(2));
    EXPECT_THROW(channel.reserve(1), std::runtime_error);
    EXPECT_THROW(channel.reserve(2), std::runtime_error);

    channel.reserve(0);
    EXPECT_FALSE(channel.can_reserve(2));
    EXPECT_THROW(channel.reserve(2), std::runtime_error);
}

TEST(QueueChannelTest, test_push) {
    mimo::QueueChannel channel(2);
    auto queue = std::make_unique<mimo::Queue>(1);

    EXPECT_THROW(channel.push(std::move(queue)), std::runtime_error);
    queue = std::make_unique<mimo::Queue>(1);
    channel.reserve(queue->run);
    channel.push(std::move(queue));
    EXPECT_FALSE(channel.has_reservation(1));
    EXPECT_FALSE(channel.can_pop());

    queue = std::make_unique<mimo::Queue>(0);
    channel.reserve(0);
    channel.push(queue);
    queue = std::make_unique<mimo::Queue>(2);
}

TEST(QueueChannelTest, test_pop) {
    mimo::QueueChannel channel;

    auto queue = std::make_unique<mimo::Queue>(2);
    channel.reserve(2);
    channel.push(std::move(queue));
    EXPECT_FALSE(channel.can_pop());

    queue = std::make_unique<mimo::Queue>(0);
    channel.reserve(0);
    channel.push(std::move(queue));
    EXPECT_TRUE(channel.can_pop());
    channel.pop();
    EXPECT_FALSE(channel.can_pop());

    queue = std::make_unique<mimo::Queue>(1);
    channel.reserve(1);
    channel.push(std::move(queue));
    EXPECT_TRUE(channel.can_pop());
    EXPECT_EQ(channel.pop()->run, 1);
    EXPECT_TRUE(channel.can_pop());
    EXPECT_EQ(channel.pop()->run, 2);
    EXPECT_FALSE(channel.can_pop());
}
