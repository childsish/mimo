/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_OUTPUTQUEUE_H
#define MIMO_OUTPUTQUEUE_H


#include "Queue.h"

namespace workflow {
    class Output;
}

namespace mimo {

    /**
     * @brief queue adaptor for queue writing functions
     */
    class OutputQueue {
    public:

        const std::shared_ptr<workflow::Output> identifier;

        explicit OutputQueue(const std::shared_ptr<workflow::Output> &identifier_) : identifier(identifier_) {
            this->run = 0;
            this->_queue = std::make_unique<mimo::Queue>();
        }

        bool push(std::shared_ptr<mimo::Entity> entity) { return this->_queue->push(entity); }
        bool can_push() const { return this->_queue->can_push(); }

        unsigned int get_run() { return this->run; }
        void end_run() { this->_queue->end_run(); }
        bool is_end_of_run() { return this->_queue->is_end_of_run(); }
        void close() { this->_queue->close(); }

        bool is_empty() const { return this->_queue->is_empty(); }

        std::unique_ptr<mimo::Queue> release_queue() {
            std::unique_ptr<mimo::Queue> queue = std::move(this->_queue);
            this->_queue = std::make_unique<mimo::Queue>();
            this->run += 1;
            return queue;
        }

    private:

        unsigned int run;

        std::unique_ptr<mimo::Queue> _queue;

    };
}


#endif //MIMO_OUTPUTQUEUE_H
