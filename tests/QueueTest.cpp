#include "gtest/gtest.h"
#include "../src/queues/Queue.h"

#include "mimo/Entity.h"
#include "../src/errors.h"

class Integer : public mimo::Entity {
public:
    int value;
    explicit Integer(int value_) : value(value_) {}
};

TEST(QueueTest, test_fifo) {
    mimo::Queue queue(2);

    queue.push(std::make_shared<Integer>(0));
    queue.push(std::make_shared<Integer>(1));

    auto integer = std::static_pointer_cast<Integer>(queue.pop());
    EXPECT_EQ(integer->value, 0);
    integer = std::static_pointer_cast<Integer>(queue.pop());
    EXPECT_EQ(integer->value, 1);
}

TEST(QueueTest, test_threshold) {
    mimo::Queue queue(2);

    EXPECT_TRUE(queue.can_push());
    EXPECT_FALSE(queue.can_pop());
    queue.push(std::make_shared<Integer>(0));
    EXPECT_TRUE(queue.can_push());
    EXPECT_TRUE(queue.can_pop());
    queue.push(std::make_shared<Integer>(0));
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

    EXPECT_THROW(queue.peek(), mimo::QueueError);
    EXPECT_THROW(queue.pop(), mimo::QueueError);
}

TEST(QueueTest, test_iterator) {
    mimo::Queue queue(10);

    for (int i = 0; i < 10; ++i) {
        queue.push(std::make_shared<Integer>(i));
    }

    int i = 0;
    for (const auto entity : queue) {
        EXPECT_EQ(std::static_pointer_cast<Integer>(entity)->value, i);
        i += 1;
    }
}
