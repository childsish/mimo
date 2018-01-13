#include "gtest/gtest.h"
#include "queues/JobInputs.h"

#include "Entity.h"
#include "errors.h"
#include "interfaces/IQueue.h"


class EmptyQueue : public mimo::IQueue {
public:
    std::shared_ptr<mimo::Entity> peek() override { throw mimo::QueueError("Trying to peek from empty queue."); }
    std::shared_ptr<mimo::Entity> pop() override { throw mimo::QueueError("Trying to pop from empty queue."); }
    void push(std::shared_ptr<mimo::Entity> entity) override {}
    bool can_pop() const override { return false; }
    bool can_push() const override { return true; }
    bool is_closed() const override { return false; }
    bool is_empty() const override { return true; }
    bool is_full() const override { return false; }
};

class FullQueue : public mimo::IQueue {
public:
    std::shared_ptr<mimo::Entity> peek() override { return std::make_shared<mimo::Entity>(); }
    std::shared_ptr<mimo::Entity> pop() override { throw mimo::QueueError("Trying to pop from empty queue."); }
    void push(std::shared_ptr<mimo::Entity> entity) override {}
    bool can_pop() const override { return true; }
    bool can_push() const override { return false; }
    bool is_closed() const override { return false; }
    bool is_empty() const override { return false; }
    bool is_full() const override { return true; }
};


TEST(InputsTest, test_asynced_queues) {
    auto queue1 = std::make_unique<EmptyQueue>();
    auto queue2 = std::make_unique<FullQueue>();

    mimo::JobInputs inputs;
    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));

    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
}

TEST(InputsTest, test_synced_queues) {
    auto queue1 = std::make_unique<FullQueue>();
    auto queue2 = std::make_unique<FullQueue>();

    mimo::JobInputs inputs;
    inputs.add_queue("queue1", std::move(queue1));
    inputs.add_queue("queue2", std::move(queue2));


    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);

    inputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);
}
