/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_OUTPUTS_H
#define MIMO_OUTPUTS_H

#include <memory>
#include <string>
#include <unordered_map>

namespace workflow {
    class Output;
}

namespace mimo {

    class Entity;
    class Queue;

    /**
     * @brief:
     */
    class Outputs {
    public:

        enum class PushStatus {
            CAN_PUSH,
            QUEUE_FULL,
            SYNC_QUEUE_FULL
        };

        Outputs(
            const std::unordered_map<std::string, std::shared_ptr<workflow::Output>> &identifiers,
            std::unordered_map<std::string, std::unique_ptr<Queue>> &queues,
            const std::unordered_map<std::string, unsigned int> &sync_groups
        );

        PushStatus get_status() const;
        PushStatus get_status(const std::string &name) const;

        void push(const std::string &name, std::shared_ptr<Entity> entity);

    private:

        unsigned int group_id;

        std::unordered_map<std::string, std::unique_ptr<Queue>> queues;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<unsigned int, bool> get_group_status() const;

    };
}


#endif //MIMO_OUTPUTS_H
