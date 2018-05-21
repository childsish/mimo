/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * A queue for storing entities between steps.
 */
#ifndef MIMO_QUEUE_H
#define MIMO_QUEUE_H

#include <deque>
#include <memory>
#include "../Factory.h"
#include "IQueue.h"


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

        std::shared_ptr<Entity> peek() override;

        std::shared_ptr<Entity> pop() override;

        void push(std::shared_ptr<Entity> entity) override;
        void push(const IQueue &queue) override;

        bool can_pop() const override;

        bool can_push() const override;

        void end_task();
        bool is_end_of_task() const;

        void close() override;
        bool is_closed() const override;

        bool is_empty() const override;

        bool is_full() const override;

        const IQueueIterator begin() const override;
        const IQueueIterator end() const override;

    private:

        unsigned int capacity;
        std::deque<std::shared_ptr<Entity>> entities;
        bool end_of_task;
        bool closed;

    };

    class QueueIterator : public IQueueIterator {
    public:
        explicit QueueIterator(std::deque::iterator iterator);
        bool operator!=(const IQueueIterator &that) override;
        IQueueIterator operator++() override;
        std::shared_ptr<Entity> operator*() override;
    private:
        std::deque::iterator iterator;
    };
}

#endif //MIMO_QUEUE_H
