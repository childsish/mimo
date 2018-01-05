#include "gtest/gtest.h"

#include <Entity.h>
#include <workflow/Input.h>
#include "queues/Inputs.h"


TEST(InputsTest, test_asynced_queues) {
    std::unique_ptr<mimo::Queue> queue2 = std::make_unique<mimo::Queue>(0);
    queue2->push(std::make_shared<mimo::Entity>());

    std::unordered_map<std::string, std::shared_ptr<workflow::Input>> input_map;
    input_map.emplace("queue1", std::make_shared<workflow::Input>(0, "queue1"));
    input_map.emplace("queue2", std::make_shared<workflow::Input>(1, "queue2"));

    mimo::Inputs inputs(input_map);
    inputs["queue2"].acquire_queue(std::move(queue2));

    EXPECT_TRUE(inputs.can_pop());

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_FALSE(inputs.can_pop());
}

TEST(InputsTest, test_synced_queues) {
    std::unique_ptr<mimo::Queue> queue1 = std::make_unique<mimo::Queue>(0);
    queue1->push(std::make_shared<mimo::Entity>());
    std::unique_ptr<mimo::Queue> queue2 = std::make_unique<mimo::Queue>(0);
    queue2->push(std::make_shared<mimo::Entity>());

    std::unordered_map<std::string, std::shared_ptr<workflow::Input>> input_map;
    input_map.emplace("queue1", std::make_shared<workflow::Input>(0, "queue1"));
    input_map.emplace("queue2", std::make_shared<workflow::Input>(1, "queue2"));

    mimo::Inputs inputs(input_map);
    inputs["queue1"].acquire_queue(std::move(queue1));
    inputs["queue2"].acquire_queue(std::move(queue2));

    EXPECT_TRUE(inputs.can_pop());

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_TRUE(inputs.can_pop());
}
