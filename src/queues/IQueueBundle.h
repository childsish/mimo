/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IQUEUEBUNDLE_H
#define MIMO_IQUEUEBUNDLE_H

#include "mimo/IInputs.h"
#include "mimo/IOutputs.h"
#include <workflow/Connection.h>
#include <memory>
#include <unordered_map>


namespace mimo {

    class Entity;
    class IQueue;
    using ConnectionMap = std::unordered_map<std::string, std::shared_ptr<workflow::Connection>>;

    class IQueueBundle : public IInputs, public IOutputs {
    public:
        virtual ~IQueueBundle() = default;

        virtual const ConnectionMap &get_identifiers() const = 0;
        virtual IQueue &get_queue(const std::string &name) = 0;
        virtual void clear() = 0;
    };
}


#endif //MIMO_IQUEUEBUNDLE_H
