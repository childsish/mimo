#ifndef MIMO_IQUEUE_H
#define MIMO_IQUEUE_H

#include <memory>


namespace mimo {

    class Entity;

    /** @brief A FIFO queue for storing entities. */
    class IQueue {
    public:
        virtual ~IQueue() = default;

        /** @brief Get the first item from the queue but leave it as the first item. */
        virtual std::shared_ptr<Entity> peek() = 0;

        /** @brief Remove the first item from the queue and return it. */
        virtual std::shared_ptr<Entity> pop() = 0;

        /** @brief Add an item to the back of the queue. */
        virtual void push(std::shared_ptr<Entity> entity) = 0;

        /** @brief Return true if the queue has items that can be popped. */
        virtual bool can_pop() const = 0;

        /** @brief Return true if the queue has sufficient capacity to hold more items. */
        virtual bool can_push() const = 0;

        /** @brief Return true if the the queue has been closed. */
        virtual bool is_closed() const = 0;

        /** @brief Return true if the queue has no items. */
        virtual bool is_empty() const = 0;

        /** @brief Return true if the queue has no more capacity. */
        virtual bool is_full() const = 0;
    };
}

#endif //MIMO_IQUEUE_H
