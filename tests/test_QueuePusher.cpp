#include "gtest/gtest.h"

#include "Queue.h"
#include "QueuePusher.h"
#include "entities/Integer.h"


TEST(QueuePusherTest, test_push) {
    Integer *integer = new Integer(0);
    Queue queue1;
    Queue queue2;
    QueuePusher pusher;
    pusher.add_queue(&queue1);
    pusher.add_queue(&queue2);

    pusher.push(integer);

    EXPECT_TRUE(queue1.can_pop());
    EXPECT_TRUE(queue2.can_pop());

    integer = static_cast<Integer *>(queue1.pop());
    EXPECT_EQ(0, integer->value);
    EXPECT_EQ(2, integer->reference_count);

    integer = static_cast<Integer *>(queue2.pop());
    EXPECT_EQ(0, integer->value);
    EXPECT_EQ(2, integer->reference_count);
    integer->reference_count -= 2; // Prevent warning message
    delete integer;
}

TEST(QueuePusherTest, test_threshold) {
    Entity *entity;
    Queue queue1(1);
    Queue queue2(2);
    QueuePusher pusher;

    EXPECT_FALSE(pusher.can_push());
    pusher.add_queue(&queue1);
    pusher.add_queue(&queue2);

    EXPECT_TRUE(pusher.can_push());
    EXPECT_FALSE(pusher.push(new Entity()));
    EXPECT_EQ(queue1.size(), 1);
    EXPECT_EQ(queue2.size(), 1);

    queue1.pop();
    entity = queue2.pop();
    entity->reference_count -= 2; // Prevent warning message
    delete entity;
}
