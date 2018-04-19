/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IQUEUEBUNDLE_H
#define MIMO_IQUEUEBUNDLE_H

#include <memory>


namespace mimo {

    class Entity;

    class IQueueBundle {
    public:

        enum class PushStatus {
            NO_QUEUE,
            CAN_PUSH,
            QUEUE_FULL,
            SYNC_QUEUE_FULL
        };

        enum class PopStatus {
            NO_QUEUE,
            CAN_POP,
            QUEUE_EMPTY,
            SYNC_QUEUE_EMPTY
        };

        virtual ~IQueueBundle() = default;

        /** @name Push functions */
        /**@{*/
        /** @brief Get whether all queues can be pushed. */
        virtual PushStatus get_push_status() const = 0;
        /** @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full. */
        virtual PushStatus get_push_status(const std::string &name) const = 0;
        /** @brief Push an entity to the queue */
        virtual void push(const std::string &name, std::shared_ptr<Entity> entity) = 0;
        /**@}*/

        /** @name Pop functions */
        /**@{*/
        /** @brief Get whether all queues can be popped. */
        virtual PopStatus get_pop_status() const = 0;
        /** @brief Get whether a queue can be popped or if it, or a synchronised queue, is empty. */
        virtual PopStatus get_pop_status(const std::string &name) const = 0;
        /** @brief Peek at the first item in the named queue but do not pop it. */
        virtual std::shared_ptr<Entity> peek(const std::string &name) = 0;
        /** @brief Pop and return the first item of the named queue. */
        virtual std::shared_ptr<Entity> pop(const std::string &name) = 0;
        /**@}*/

        /** @brief Close all queues. */
        virtual void close() = 0;
        /** @brief Get whether all queues are closed. */
        virtual bool is_closed() const = 0;
    };
}


#endif //MIMO_IQUEUEBUNDLE_H
