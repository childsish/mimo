/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBINPUT_H
#define MIMO_JOBINPUT_H

#include <memory>
#include <unordered_map>


namespace mimo {

    class Entity;
    class Queue;

    /**
     * @brief: A set of input queues to a step.
     */
    class JobInputs {
    public:

        enum class PopStatus {
            CAN_POP,
            QUEUE_EMPTY,
            SYNC_QUEUE_EMPTY
        };

        JobInputs(
            std::unordered_map<std::string, std::unique_ptr<Queue>> &queues,
            const std::unordered_map<std::string, unsigned int> &sync_groups
        );

        PopStatus get_status() const;
        PopStatus get_status(const std::string &name) const;

        std::shared_ptr<Entity> &peek(const std::string &name);
        std::shared_ptr<Entity> pop(const std::string &name);

        bool is_empty() const;
        bool is_closed() const;

    private:

        unsigned int group_id;

        std::unordered_map<std::string, std::unique_ptr<mimo::Queue>> queues;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<unsigned int, bool> get_group_status() const;
    };
}


#endif //MIMO_JOBINPUT_H
