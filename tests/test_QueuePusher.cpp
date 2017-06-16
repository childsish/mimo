#include "gtest/gtest.h"

#include "Queue.h"
#include "QueuePusher.h"


class Integer : public Entity {
public:
    int value;

    Integer(int value_) : value(value_) {}
};

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
    delete integer;
}

TEST(QueuePusherTest, test_threshold) {
    Queue queue1(1);
    Queue queue2(2);
    QueuePusher pusher;

    EXPECT_FALSE(pusher.can_push());
    pusher.add_queue(&queue1);
    pusher.add_queue(&queue2);
    EXPECT_TRUE(pusher.can_push());
    EXPECT_FALSE(pusher.push(new Entity()));

    delete queue1.pop();
}
