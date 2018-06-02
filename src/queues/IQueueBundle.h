/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IQUEUEBUNDLE_H
#define MIMO_IQUEUEBUNDLE_H

#include <memory>
#include "mimo/IInputs.h"
#include "mimo/IOutputs.h"


namespace mimo {

    class Entity;
    using ConnectionMap = std::unordered_map<std::string, std::shared_ptr<workflow::Connection>>;

    class IQueueBundle : public IInputs, public IOutputs {
    public:
        virtual ~IQueueBundle() = default;

        virtual const ConnectionMap &get_identifiers() const = 0;

        virtual std::unique_ptr<IQueue> release_queue(
            const workflow::Connection &id
        ) = 0;

        virtual void acquire_queue(
            const workflow::Connection &id,
            std::unique_ptr<IQueue> queue
        ) = 0;

        virtual void push(const std::string &name, const IQueue &queue) = 0;
    };
}


#endif //MIMO_IQUEUEBUNDLE_H
