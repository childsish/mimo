/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_OUTPUTQUEUE_H
#define MIMO_OUTPUTQUEUE_H


#include <workflow/Output.h>
#include "Queue.h"

namespace mimo {

    /**
     * @brief queue adaptor for queue writing functions
     */
    class OutputQueue {
    public:

        const std::shared_ptr<workflow::Output> identifier;

        const unsigned int run;

        explicit OutputQueue(const std::shared_ptr<workflow::Output> &identifier_, unsigned int run_) :
            identifier(identifier_),
            run(run_)
        {
            this->_queue = std::make_unique<mimo::Queue>();
        }

        bool push(std::shared_ptr<mimo::Entity> entity) { return this->_queue->push(entity); }
        bool can_push() const { return this->_queue->can_push(); }

        void end_run() { this->_queue->end_run(); }
        void close() { this->_queue->close(); }

        bool is_empty() const { return this->_queue->is_empty(); }

        std::unique_ptr<mimo::Queue> release_queue() {
            std::unique_ptr<mimo::Queue> queue = std::move(this->_queue);
            this->_queue = std::make_unique<mimo::Queue>();
            return queue;
        }

    private:

        std::unique_ptr<mimo::Queue> _queue;

    };
}


#endif //MIMO_OUTPUTQUEUE_H
