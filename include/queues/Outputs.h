/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_OUTPUTS_H
#define MIMO_OUTPUTS_H

#include <memory>
#include <string>
#include "queues/IQueueBundle.h"

namespace workflow {
    class Output;
}

namespace mimo {

    class Entity;

    /** @brief: Provides access to output queues. */
    class Outputs {
    public:

        enum class PushStatus {
            CAN_PUSH = static_cast<int>(IQueueBundle::PushStatus::CAN_PUSH),
            QUEUE_FULL = static_cast<int>(IQueueBundle::PushStatus::QUEUE_FULL),
            SYNC_QUEUE_FULL = static_cast<int>(IQueueBundle::PushStatus::SYNC_QUEUE_FULL)
        };

        explicit Outputs(std::shared_ptr<IQueueBundle> &outputs);

        PushStatus get_status() const;
        PushStatus get_status(const std::string &name) const;

        void push(const std::string &name, std::shared_ptr<Entity> entity);

    private:

        std::shared_ptr<IQueueBundle> &outputs;

    };
}


#endif //MIMO_OUTPUTS_H
