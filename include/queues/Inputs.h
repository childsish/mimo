/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <memory>
#include <string>
#include "queues/IJobInputs.h"

namespace workflow {
    class Input;
}

namespace mimo {

    class Entity;
    class Queue;

    /**
     * @brief: Provides access to input queues.
     */
    class Inputs {
    public:

        enum class PopStatus {
            CAN_POP = static_cast<int>(IJobInputs::PopStatus::CAN_POP),
            QUEUE_EMPTY = static_cast<int>(IJobInputs::PopStatus::QUEUE_EMPTY),
            SYNC_QUEUE_EMPTY = static_cast<int>(IJobInputs::PopStatus::SYNC_QUEUE_EMPTY)
        };

        explicit Inputs(std::unique_ptr<IJobInputs> &inputs);

        PopStatus get_status() const;
        PopStatus get_status(const std::string &name) const;

        std::shared_ptr<Entity> peek(const std::string &name);
        std::shared_ptr<Entity> pop(const std::string &name);

    private:

        std::unique_ptr<IJobInputs> &inputs;

    };
}


#endif //MIMO_INPUTS_H
