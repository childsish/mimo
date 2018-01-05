/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_OUTPUTS_H
#define MIMO_OUTPUTS_H

#include <memory>
#include <string>
#include "queues/JobOutputs.h"

namespace workflow {
    class Output;
}

namespace mimo {

    class Entity;
    class Queue;

    /**
     * @brief: Provides access to output queues.
     */
    class Outputs {
    public:

        enum class PushStatus {
            CAN_PUSH = static_cast<int>(JobOutputs::PushStatus::CAN_PUSH),
            QUEUE_FULL = static_cast<int>(JobOutputs::PushStatus::QUEUE_FULL),
            SYNC_QUEUE_FULL = static_cast<int>(JobOutputs::PushStatus::SYNC_QUEUE_FULL)
        };

        explicit Outputs(JobOutputs &outputs);

        PushStatus get_status() const;
        PushStatus get_status(const std::string &name) const;

        void push(const std::string &name, std::shared_ptr<Entity> entity);

    private:

        JobOutputs &outputs;

    };
}


#endif //MIMO_OUTPUTS_H
