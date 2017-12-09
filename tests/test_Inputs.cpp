#include "gtest/gtest.h"

#include "queues/Inputs.h"


template<bool CAN_POP>
class CanPopQueue : public mimo::Queue {
public:
    CanPopQueue() : Queue(0) {}

    bool can_pop() const { return CAN_POP; }
};


TEST(InputsTest, test_name) {
    mimo::Inputs inputs;
    std::unique_ptr<mimo::Queue> queue1 = std::make_unique<CanPopQueue<true>>();
    std::unique_ptr<mimo::Queue> queue2 = std::make_unique<CanPopQueue<true>>();
    std::unique_ptr<mimo::Queue> queue3 = std::make_unique<CanPopQueue<true>>();

    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));
    EXPECT_TRUE(inputs.can_pop());

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_FALSE(inputs.can_pop());

    inputs.add_queue("queue3", std::move(queue3));
    EXPECT_TRUE(inputs.can_pop());
}
