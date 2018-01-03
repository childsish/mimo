/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTQUEUE_H
#define MIMO_INPUTQUEUE_H


#include "Queue.h"

namespace workflow {
    class Input;
}

namespace mimo {

    /**
     * @brief queue adaptor for queue reading functions
     */
    class InputQueue {
    public:

        const std::shared_ptr<workflow::Input> identifier;

        explicit InputQueue(const std::shared_ptr<workflow::Input> &identifier_) : identifier(identifier_) {
            this->_queue = std::make_unique<mimo::Queue>();
        }

        std::shared_ptr<mimo::Entity> peek() { return _queue->peek(); }
        std::shared_ptr<mimo::Entity> pop() { return _queue->pop(); }
        bool can_pop() const { return _queue->can_pop(); }

        bool is_closed() const { return _queue->is_closed(); }

        void acquire_queue(std::unique_ptr<Queue> queue) {
            this->_queue = std::move(queue);
        }

    private:

        std::unique_ptr<mimo::Queue> _queue;

    };
}


#endif //MIMO_INPUTQUEUE_H
