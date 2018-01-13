#ifndef MIMO_IQUEUE_H
#define MIMO_IQUEUE_H

#include <memory>


namespace mimo {

    class Entity;

    class IQueue {
    public:

        virtual std::shared_ptr<Entity> peek() = 0;
        virtual std::shared_ptr<Entity> pop() = 0;
        virtual void push(std::shared_ptr<Entity> entity) = 0;
        virtual bool can_pop() const = 0;
        virtual bool can_push() const = 0;
        virtual bool is_closed() const = 0;
        virtual bool is_empty() const = 0;
        virtual bool is_full() const = 0;
    };
}

#endif //MIMO_IQUEUE_H
