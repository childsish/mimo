#include "gtest/gtest.h"

#include "Entity.h"
#include "queues/Queue.h"
#include "entities/Integer.h"


TEST(QueueTest, test_fifo) {
    mimo::Queue queue(0);

    queue.push(std::static_pointer_cast<mimo::Entity>(std::make_shared<Integer>(0)));
    queue.push(std::static_pointer_cast<mimo::Entity>(std::make_shared<Integer>(1)));

    auto integer = std::static_pointer_cast<Integer>(queue.pop());
    EXPECT_EQ(integer->value, 0);
    integer = std::static_pointer_cast<Integer>(queue.pop());
    EXPECT_EQ(integer->value, 1);
}

TEST(QueueTest, test_treshold) {
    mimo::Queue queue(0, 2);

    EXPECT_TRUE(queue.can_push());
    EXPECT_FALSE(queue.can_pop());
    EXPECT_TRUE(queue.push(std::static_pointer_cast<mimo::Entity>(std::make_shared<Integer>(0))));
    EXPECT_TRUE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    EXPECT_FALSE(queue.push(std::static_pointer_cast<mimo::Entity>(std::make_shared<Integer>(0))));
    EXPECT_FALSE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    auto entity = queue.pop();
    EXPECT_TRUE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    entity = queue.pop();
    EXPECT_FALSE(queue.can_pop());
}

TEST(QueueTest, test_empty) {
    mimo::Queue queue(0);

    EXPECT_THROW(queue.peek(), std::runtime_error);
    EXPECT_THROW(queue.pop(), std::runtime_error);
}
