/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_IJOBOUTPUTS_H
#define MIMO_IJOBOUTPUTS_H

#include <memory>
#include <unordered_map>
#include <vector>


namespace mimo {

    class Entity;
    class IQueue;
    class IQueueFactory;

    class IJobOutputs {
    public:

        enum class PushStatus {
            CAN_PUSH,
            QUEUE_FULL,
            SYNC_QUEUE_FULL
        };

        /**
         * @brief Get named queue from outputs.
         */
        virtual std::unique_ptr <IQueue> get_queue(const std::string name) = 0;

        /**
         * @brief Synchronise the named queues.
         */
        virtual void synchronise_queues(const std::vector <std::string> &queues) = 0;

        /**
         * @brief Get whether all queues can be pushed.
         */
        virtual PushStatus get_status() const = 0;

        /**
         * @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full.
         * @param name Queue to query.
         */
        virtual PushStatus get_status(const std::string &name) const = 0;

        /**
         * @brief Push an entity onto the specified queue.
         * If the queue is full or a synchronised queue is full, an exception is thrown.
         * @param name
         * @param entity
         */
        virtual void push(const std::string &name, std::shared_ptr <Entity> entity) = 0;

        virtual void end_run() = 0;

        virtual void close() = 0;

        virtual bool is_closed() const = 0;
    };
}


#endif //MIMO_IJOBOUTPUTS_H
