/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTQUEUE_H
#define MIMO_INPUTQUEUE_H


#include "Queue.h"

namespace mimo {

    /**
     * @brief queue adaptor for queue reading functions
     */
    class InputQueue {
    public:

        InputQueue(std::unique_ptr<Queue> queue) : _queue(std::move(queue)) {}

        std::shared_ptr<mimo::Entity> peek() { return _queue->peek(); }
        std::shared_ptr<mimo::Entity> pop() { return _queue->pop(); }
        bool can_pop() const { return _queue->can_pop(); }

        std::unique_ptr<Queue> &release_queue() { return this->_queue; }

    private:

        std::unique_ptr<Queue> _queue;

    };
}


#endif //MIMO_INPUTQUEUE_H
