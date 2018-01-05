#include "gtest/gtest.h"

#include "queues/Outputs.h"


TEST(OutputsTest, test_asynced_queues) {
    std::unordered_map<std::string, std::shared_ptr<workflow::Output>> output_map;
    output_map.emplace("queue1", std::make_shared<workflow::Output>(0, "queue1"));
    output_map.emplace("queue2", std::make_shared<workflow::Output>(1, "queue2"));

    mimo::Outputs outputs(output_map);

    outputs.add_queue("queue1", std::move(queue1));
    outputs.add_queue("queue2", std::move(queue2));
    EXPECT_TRUE(outputs.can_push());

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_FALSE(outputs.can_push());

    outputs.add_queue("queue3", std::move(queue3));
    EXPECT_TRUE(outputs.can_push());
}

TEST(OutputsTest, test_synced_queues) {
    mimo::Outputs outputs;
    std::unique_ptr<mimo::Queue> queue1 = std::make_unique<mimo::Queue>(0, 1);
    std::unique_ptr<mimo::Queue> queue2 = std::make_unique<mimo::Queue>(0, 1);

    outputs.add_queue("queue1", std::move(queue1));
    outputs.add_queue("queue2", std::move(queue2));
    EXPECT_TRUE(outputs.can_push());

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_TRUE(outputs.can_push());
}
