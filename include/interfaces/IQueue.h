#ifndef MIMO_IQUEUE_H
#define MIMO_IQUEUE_H

#include <memory>


namespace mimo {

    class Entity;

    class IQueue {
    public:

        std::shared_ptr<Entity> &peek();
        std::shared_ptr<Entity> pop();
        void push(std::shared_ptr<Entity> entity);
    };
}

#endif //MIMO_IQUEUE_H
