/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * A queue for storing entities between steps.
 */
#ifndef MIMO_QUEUE_H
#define MIMO_QUEUE_H

#include <memory>
#include <queue>
#include "Factory.h"
#include "queues/IQueue.h"


namespace mimo {

    class Entity;

    class Queue : public IQueue {
    public:

        static unsigned int CAPACITY;

        /**
         * Create a queue with a given threshold.
         * Copy construction is explicitly deleted.
         * @param capacity maximum
         */
        explicit Queue(unsigned int capacity = CAPACITY);

        /**
         * Get the next entity, but don't remove it from the queue.
         * @return next entity
         */
        std::shared_ptr<Entity> peek() override;

        /**
         * Get the next entity and remove it from the queue.
         * @return next entity
         */
        std::shared_ptr<Entity> pop() override;

        /**
         * Push an entity into the queue. Return true if there is still space for more.
         * @param entity entity to push
         * @return true if queue size is less than threshold
         */
        void push(std::shared_ptr<Entity> entity) override;

        /**
         * Check if queue can be popped from
         * @return true if entities in queue
         */
        bool can_pop() const override;

        /**
         * Check if queue can be pushed to
         * @return true if fewer entities in queue than threshold
         */
        bool can_push() const override;

        /**
         * @brief flag queue as end-of-task
         * Indicate that this queue is the last to be produced in this task.
         * Queue can no longer be pushed to and can only be popped until it's empty.
         */
        void end_task();
        bool is_end_of_task() const;

        /**
         * @brief flag queue as end-of-stream
         * Indicate that this queue is the last to be produced by this step.
         * Queue can no longer be pushed to and can only be popped until it's empty.
         */
        void close();
        bool is_closed() const override;

        /**
         * Check if the queue is full
         * @return true if the queue is full
         * @return
         */
        bool is_full() const override;

    private:

        unsigned int capacity;

        std::queue<std::shared_ptr<Entity>> entities;

        bool end_of_task;

        bool closed;

    };
}

#endif //MIMO_QUEUE_H
