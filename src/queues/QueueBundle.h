/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_QUEUEBUNDLE_H
#define MIMO_QUEUEBUNDLE_H

#include <workflow/Step.h>
#include <workflow/Connection.h>
#include "QueueFactory.h"
#include "IQueueBundle.h"


namespace mimo {

    class IQueue;
    using QueueMap = std::unordered_map<std::string, std::unique_ptr<IQueue>>;

    class QueueBundle : public IQueueBundle {
    public:

        explicit QueueBundle(
            std::shared_ptr<ConnectionMap> identifiers,
            std::shared_ptr<IFactory<IQueue>> factory = std::make_shared<QueueFactory>()
        );

        const ConnectionMap &get_identifiers() const override;
        IQueue &get_queue(const std::string &name) override;

        PushStatus get_push_status() const override;
        PushStatus get_push_status(const std::string &name) const override;
        void push(const std::string &name, std::shared_ptr<Entity> entity) override;

        PopStatus get_pop_status() const override;
        PopStatus get_pop_status(const std::string &name) const override;
        std::shared_ptr<Entity> peek(const std::string &name) override;
        std::shared_ptr<Entity> pop(const std::string &name) override;

        void close() override;
        bool is_closed(const std::string& name) const override;
        void clear() override;

    private:

        std::shared_ptr<ConnectionMap> identifiers;
        std::shared_ptr<IFactory<IQueue>> factory;
        QueueMap queues;

        std::unordered_map<unsigned int, bool> get_group_push_status() const;

        std::unordered_map<unsigned int, bool> get_group_pop_status() const;

    };
}


#endif //MIMO_QUEUEBUNDLE_H
