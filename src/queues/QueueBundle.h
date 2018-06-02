/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLE_H
#define MIMO_QUEUEBUNDLE_H

#include <workflow/Step.h>
#include <workflow/Connection.h>
#include "QueueFactory.h"
#include "IQueueBundle.h"


namespace mimo {

    using QueueMap = std::unordered_map<std::string, std::unique_ptr<IQueue>>;

    class QueueBundle : public IQueueBundle {
    public:

        explicit QueueBundle(
            std::shared_ptr<ConnectionMap> identifiers,
            std::shared_ptr<IQueueFactory> factory = std::make_shared<QueueFactory>()
        );

        const ConnectionMap &get_identifiers() const override;

        std::unique_ptr<IQueue> release_queue(
            const workflow::Connection &id
        ) override;

        void acquire_queue(
            const workflow::Connection &id,
            std::unique_ptr<IQueue> queue
        ) override;

        PushStatus get_push_status() const override;
        PushStatus get_push_status(const std::string &name) const override;
        void push(const std::string &name, std::shared_ptr<Entity> entity) override;
        void push(const std::string &name, const IQueue &queue) override;

        PopStatus get_pop_status() const override;
        PopStatus get_pop_status(const std::string &name) const override;
        std::shared_ptr<Entity> peek(const std::string &name) override;
        std::shared_ptr<Entity> pop(const std::string &name) override;

    private:

        std::shared_ptr<ConnectionMap> identifiers;
        std::shared_ptr<IQueueFactory> factory;
        QueueMap queues;

        std::unordered_map<unsigned int, bool> get_group_push_status() const;

        std::unordered_map<unsigned int, bool> get_group_pop_status() const;

    };
}


#endif //MIMO_QUEUEBUNDLE_H
