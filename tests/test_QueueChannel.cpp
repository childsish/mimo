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

    EXPECT_TRUE(channel.can_push());

    auto queue = std::make_unique<mimo::Queue>();
    EXPECT_THROW(channel.push(std::move(queue), 0), std::runtime_error);

    queue = std::make_unique<mimo::Queue>();
    auto identifier = channel.reserve();
    channel.push(std::move(queue), identifier);
    EXPECT_FALSE(channel.can_push());
    EXPECT_TRUE(channel.can_pop());
}

TEST(QueueChannelTest, test_pop) {
    mimo::QueueChannel channel(1);

    auto queue = std::make_unique<mimo::Queue>();
    auto identifier = channel.reserve();
    channel.push(std::move(queue), identifier);

    EXPECT_TRUE(channel.can_pop());
    queue = channel.pop();
    EXPECT_FALSE(channel.can_pop());
    EXPECT_TRUE(channel.can_push());
}
