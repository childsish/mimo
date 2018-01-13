#include "gtest/gtest.h"
#include "queues/Outputs.h"

#include <queue>
#include "errors.h"
#include "Entity.h"
#include "interfaces/IQueue.h"
#include "interfaces/IQueueFactory.h"


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

class QueueFactory : public mimo::IQueueFactory {
public:

    explicit QueueFactory(std::queue<std::unique_ptr<mimo::IQueue>> &queues_) : queues(std::move(queues_)) {}

    std::unique_ptr<mimo::IQueue> make() override {
        auto queue = std::move(this->queues.front());
        this->queues.pop();
        return queue;
    };

private:

    std::queue<std::unique_ptr<mimo::IQueue>> queues;
};

TEST(OutputsTest, test_asynced_queues) {
    std::queue<std::unique_ptr<mimo::IQueue>> queues;
    queues.push(std::make_unique<EmptyQueue>());
    queues.push(std::make_unique<FullQueue>());
    QueueFactory factory(queues);

    mimo::JobOutputs outputs(factory, {"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::QUEUE_FULL);

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::QUEUE_FULL);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::SYNC_QUEUE_FULL);
}

TEST(OutputsTest, test_synced_queues) {
    std::queue<std::unique_ptr<mimo::IQueue>> queues;
    queues.push(std::make_unique<EmptyQueue>());
    queues.push(std::make_unique<FullQueue>());
    QueueFactory factory(queues);

    mimo::JobOutputs outputs(factory, {"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);

    outputs.synchronise_queues({"queue1", "queue2"});
    EXPECT_EQ(outputs.get_status(), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue1"), mimo::JobOutputs::PushStatus::CAN_PUSH);
    EXPECT_EQ(outputs.get_status("queue2"), mimo::JobOutputs::PushStatus::CAN_PUSH);
}
