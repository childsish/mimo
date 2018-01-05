/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBOUTPUT_H
#define MIMO_JOBOUTPUT_H

#include <string>
#include <unordered_map>


namespace mimo {

    class Entity;
    class Queue;

    /**
     * @brief A set of output queues from a step.
     */
    class JobOutputs {
    public:

        enum class PushStatus {
            CAN_PUSH,
            QUEUE_FULL,
            SYNC_QUEUE_FULL
        };

        JobOutputs(
            std::unordered_map<std::string, std::unique_ptr<Queue>> &queues,
            const std::unordered_map<std::string, unsigned int> &sync_groups
        );

        /**
         * @brief Get whether all queues can be pushed.
         */
        PushStatus get_status() const;

        /**
         * @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full.
         * @param name Queue to query.
         */
        PushStatus get_status(const std::string &name) const;

        /**
         * @brief Push an entity onto the specified queue.
         * If the queue is full or a synchronised queue is full, an exception is thrown.
         * @param name
         * @param entity
         */
        void push(const std::string &name, std::shared_ptr<Entity> entity);

    private:

        std::unordered_map<std::string, std::unique_ptr<Queue>> &queues;

        const std::unordered_map<std::string, unsigned int> &sync_groups;

        std::unordered_map<unsigned int, bool> get_group_status() const;

    };
}


#endif //MIMO_JOBOUTPUT_H
