/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_OUTPUTS_H
#define MIMO_OUTPUTS_H

#include <memory>
#include <string>


namespace mimo {

    class Entity;

    /** @brief: Provides access to output queues. */
    class IOutputs {
    public:
        enum class PushStatus {
            CAN_PUSH,
            NO_QUEUE,
            QUEUE_FULL,
            SYNC_QUEUE_FULL
        };

        virtual ~IOutputs() = default;

        virtual PushStatus get_push_status() const = 0;
        virtual PushStatus get_push_status(const std::string &name) const = 0;

        virtual void push(const std::string &name, std::shared_ptr<Entity> entity) = 0;
    };
}


#endif //MIMO_OUTPUTS_H
