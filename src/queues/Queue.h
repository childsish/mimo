/** @author Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUE_H
#define MIMO_QUEUE_H

#include <deque>
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

        unsigned long get_size() const override;

        bool can_pop() const override;
        std::shared_ptr<Entity> peek() override;
        std::shared_ptr<Entity> pop() override;

        bool can_push() const override;
        void push(std::shared_ptr<Entity> entity) override;
        void push(const IQueue &queue) override;

        void clear() override;

        void close() override;
        bool is_closed() const override;

        IForwardIterator<std::shared_ptr<Entity>> begin() override;
        IForwardIterator<std::shared_ptr<Entity>> begin() const override;
        IForwardIterator<std::shared_ptr<Entity>> end() override;
        IForwardIterator<std::shared_ptr<Entity>> end() const override;

    private:

        unsigned int capacity;
        bool closed;
        std::deque<std::shared_ptr<Entity>> entities;

    };
}

#endif //MIMO_QUEUE_H
