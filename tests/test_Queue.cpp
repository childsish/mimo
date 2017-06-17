#include "gtest/gtest.h"

#include "Entity.h"
#include "Queue.h"
#include "entities/Integer.h"


TEST(QueueTest, test_fifo) {
    Queue queue;
    Integer *integer;

    queue.push(new Integer(0));
    queue.push(new Integer(1));

    integer = static_cast<Integer *>(queue.pop());
    EXPECT_EQ(integer->value, 0);
    integer->reference_count--; // Prevent warning message
    delete integer;
    integer = static_cast<Integer *>(queue.pop());
    EXPECT_EQ(integer->value, 1);
    integer->reference_count--; // Prevent warning message
    delete integer;
}

TEST(QueueTest, test_treshold) {
    Entity *entity;
    Queue queue(2);

    EXPECT_TRUE(queue.can_push());
    EXPECT_FALSE(queue.can_pop());
    EXPECT_TRUE(queue.push(new Integer(0)));
    EXPECT_TRUE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    EXPECT_FALSE(queue.push(new Integer(0)));
    EXPECT_FALSE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    entity = queue.pop();
    entity->reference_count--; // Prevent warning message
    delete entity;
    EXPECT_TRUE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    entity = queue.pop();
    entity->reference_count--; // Prevent warning message
    delete entity;
    EXPECT_FALSE(queue.can_pop());
}

TEST(QueueTest, test_empty) {
    Queue queue;

    EXPECT_THROW(queue.peek(), std::runtime_error);
    EXPECT_THROW(queue.pop(), std::runtime_error);
}
