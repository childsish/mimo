/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <memory>
#include <string>
#include <workflow/Input.h>
#include "../src/queues/IQueueBundle.h"


namespace mimo {

    class Entity;

    /** @brief: Provides access to input queues. */
    class Inputs {
    public:

        enum class PopStatus {
            CAN_POP = static_cast<int>(IQueueBundle::PopStatus::CAN_POP),
            QUEUE_EMPTY = static_cast<int>(IQueueBundle::PopStatus::QUEUE_EMPTY),
            SYNC_QUEUE_EMPTY = static_cast<int>(IQueueBundle::PopStatus::SYNC_QUEUE_EMPTY)
        };

        explicit Inputs(std::shared_ptr<IQueueBundle> &inputs);

        PopStatus get_status() const;
        PopStatus get_status(const std::string &name) const;

        std::shared_ptr<Entity> peek(const std::string &name);
        std::shared_ptr<Entity> pop(const std::string &name);

    private:

        std::shared_ptr<IQueueBundle> &inputs;

    };
}


#endif //MIMO_INPUTS_H
