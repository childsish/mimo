/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * A queue for storing entities between steps.
 */
#ifndef MIMO_QUEUE_H
#define MIMO_QUEUE_H


#include <memory>
#include <queue>
#include "Entity.h"

namespace mimo {

    class Queue {
    public:

        static unsigned int CAPACITY;

        const unsigned int run;

        /**
         * Create a queue with a given threshold.
         * Copy construction is explicitly deleted.
         * @param capacity maximum
         */
        explicit Queue(unsigned int run, unsigned int capacity = CAPACITY);
        //Queue(const Queue &) = delete;
        //~Queue();

        /**
         * Push an entity into the queue. Return true if there is still space for more.
         * @param entity entity to push
         * @return true if queue size is less than threshold
         */
        bool push(std::shared_ptr<mimo::Entity> entity);

        /**
         * Get the next entity, but don't remove it from the queue.
         * @return next entity
         */
        std::shared_ptr<mimo::Entity> peek();

        /**
         * Get the next entity and remove it from the queue.
         * @return next entity
         */
        std::shared_ptr<mimo::Entity> pop();

        /**
         * Close the queue.
         * Queue can no longer be pushed to and can only be popped until it's empty.
         */
        void close();
        bool closed() const;

        /**
         * Check if queue can be pushed to
         * @return true if fewer entities in queue than threshold
         */
        bool can_push() const;

        /**
         * Check if queue can be popped from
         * @return true if entities in queue
         */
        bool can_pop() const;

    private:

        unsigned int capacity;

        std::queue<std::shared_ptr<mimo::Entity>> entities;

        bool _closed;

    };
}

#endif //MIMO_QUEUE_H
