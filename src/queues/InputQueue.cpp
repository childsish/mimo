/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <queues/InputQueue.h>
#include <queues/Queue.h>


mimo::InputQueue::InputQueue(const std::shared_ptr<workflow::Input> &identifier_, std::unique_ptr<mimo::Queue> queue_) :
    identifier(identifier_),
    queue(std::move(queue_))
{
    this->queue = std::make_unique<mimo::Queue>();
}

std::shared_ptr<mimo::Entity> mimo::InputQueue::peek() {
    return this->queue->peek();
}

std::shared_ptr<mimo::Entity> mimo::InputQueue::pop() {
    return this->queue->pop();
}

bool mimo::InputQueue::can_pop() const {
    return queue->can_pop();
}

bool mimo::InputQueue::is_closed() const {
    return queue->is_closed();
}
