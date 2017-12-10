/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_OUTPUTQUEUE_H
#define MIMO_OUTPUTQUEUE_H


#include "Queue.h"

namespace mimo {

    /**
     * @brief queue adaptor for queue writing functions
     */
    class OutputQueue {
    public:

        OutputQueue(std::unique_ptr<Queue> queue) : _queue(std::move(queue)) {}

        bool push(std::shared_ptr<mimo::Entity> entity) { return this->_queue->push(entity); }
        bool can_push() const { return this->_queue->can_push(); }

        std::unique_ptr<Queue> release_queue() { return std::move(this->_queue); }

    private:

        std::unique_ptr<Queue> _queue;

    };
}


#endif //MIMO_OUTPUTQUEUE_H
