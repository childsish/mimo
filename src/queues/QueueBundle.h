/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLE_H
#define MIMO_QUEUEBUNDLE_H

#include <workflow/Step.h>
#include <workflow/Connection.h>
#include "QueueFactory.h"
#include "IQueueBundle.h"


namespace mimo {

    using QueueMap = std::unordered_map<std::string, std::unique_ptr<IQueue>>;
    using ConnectionMap = std::unordered_map<std::string, std::shared_ptr<workflow::Connection>>;

    class QueueBundle : public IQueueBundle {
    public:

        explicit QueueBundle(
            const workflow::InputMap &identifiers
        );

        explicit QueueBundle(
            const workflow::OutputMap &identifiers,
            std::shared_ptr<IQueueFactory> factory = std::make_shared<QueueFactory>()
        );

        void acquire_queue(
            const std::shared_ptr<workflow::Connection> &connection_id,
            std::unique_ptr<IQueue> queue
        ) override;

        std::unique_ptr<IQueue> release_queue(
            const std::shared_ptr<workflow::Connection> &connection_id
        ) override;

        /** @name Push functions */
        /**@{*/
        /** @brief Get whether all queues can be pushed. */
        PushStatus get_push_status() const override;
        /** @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full. */
        PushStatus get_push_status(const std::string &name) const override;
        /** @brief Push an entity to the queue */
        void push(const std::string &name, std::shared_ptr<Entity> entity) override;
        /**@}*/

        /** @name Pop functions */
        /**@{*/
        /** @brief Get whether all queues can be popped. */
        PopStatus get_pop_status() const override;
        /** @brief Get whether a queue can be popped or if it, or a synchronised queue, is empty. */
        PopStatus get_pop_status(const std::string &name) const override;
        /** @brief Peek at the first item in the named queue but do not pop it. */
        std::shared_ptr<Entity> peek(const std::string &name) override;
        /** @brief Pop and return the first item of the named queue. */
        std::shared_ptr<Entity> pop(const std::string &name) override;
        /**@}*/

    private:

        ConnectionMap identifiers;
        QueueMap queues;

        std::unordered_map<unsigned int, bool> get_group_push_status() const;

        std::unordered_map<unsigned int, bool> get_group_pop_status() const;

    };
}


#endif //MIMO_QUEUEBUNDLE_H
