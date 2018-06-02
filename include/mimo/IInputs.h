/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <memory>
#include <string>


namespace mimo {

    class Entity;

    /** @brief: Provides access to input queues. */
    class IInputs {
    public:
        enum class PopStatus {
            CAN_POP,
            NO_QUEUE,
            QUEUE_EMPTY,
            SYNC_QUEUE_EMPTY
        };

        virtual PopStatus get_pop_status() const = 0;
        virtual PopStatus get_pop_status(const std::string &name) const = 0;

        virtual std::shared_ptr<Entity> peek(const std::string &name) = 0;
        virtual std::shared_ptr<Entity> pop(const std::string &name) = 0;
    };
}


#endif //MIMO_INPUTS_H
