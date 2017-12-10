#include "gtest/gtest.h"

#include "queues/Inputs.h"


TEST(InputsTest, test_asynced_queues) {
    mimo::Inputs inputs;
    std::unique_ptr<mimo::Queue> queue1 = std::make_unique<mimo::Queue>(0);
    std::unique_ptr<mimo::Queue> queue2 = std::make_unique<mimo::Queue>(0);
    queue2->push(std::make_shared<mimo::Entity>());
    std::unique_ptr<mimo::Queue> queue3 = std::make_unique<mimo::Queue>(0);
    queue3->push(std::make_shared<mimo::Entity>());

    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));
    EXPECT_TRUE(inputs.can_pop());

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_FALSE(inputs.can_pop());

    inputs.add_queue("queue3", std::move(queue3));
    EXPECT_TRUE(inputs.can_pop());
}

TEST(InputsTest, test_synced_queues) {
    mimo::Inputs inputs;
    std::unique_ptr<mimo::Queue> queue1 = std::make_unique<mimo::Queue>(0);
    queue1->push(std::make_shared<mimo::Entity>());
    std::unique_ptr<mimo::Queue> queue2 = std::make_unique<mimo::Queue>(0);
    queue2->push(std::make_shared<mimo::Entity>());

    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));
    EXPECT_TRUE(inputs.can_pop());

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_TRUE(inputs.can_pop());
}
