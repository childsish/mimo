/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_IJOBINPUTS_H
#define MIMO_IJOBINPUTS_H

#include <memory>
#include <unordered_map>
#include <vector>


namespace mimo {

    class Entity;
    class IQueue;

    /**
     * @brief: A set of input queues to a step.
     */
    class IJobInputs {
    public:

        enum class PopStatus {
            CAN_POP,
            QUEUE_EMPTY,
            SYNC_QUEUE_EMPTY
        };

        /**
         * @brief Add a queue to the inputs.
         * @param name Name of the queue.
         * @param queue The queue.
         */
        virtual void add_queue(const std::string &name, std::unique_ptr<IQueue> queue) = 0;

        /**
         * @brief Synchronise the named queues.
         */
        virtual void synchronise_queues(const std::vector<std::string> &queues) = 0;

        /**
         * Get whether all queues can be popped.
         * @return
         */
        virtual PopStatus get_status() const = 0;

        /**
         * @brief Get whether a queue can be popped or if it, or a synchronised queue, is empty.
         * @param name Queue to query.
         */
        virtual PopStatus get_status(const std::string &name) const = 0;

        /**
         * @brief Peek at the first item in the named queue but do not pop it.
         */
        virtual std::shared_ptr<Entity> peek(const std::string &name) = 0;

        /**
         * @brief Pop and return the first item of the named queue.
         */
        virtual std::shared_ptr<Entity> pop(const std::string &name) = 0;

        /**
         * @brief Get whether there are any queues in the inputs.
         */
        virtual bool is_empty() const = 0;

        /**
         * @brief Get whether all queues are closed.
         */
        virtual bool is_closed() const = 0;
    };
}


#endif //MIMO_IJOBINPUTS_H
