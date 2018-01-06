/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBOUTPUTS_H
#define MIMO_JOBOUTPUTS_H

#include <string>
#include <memory>
#include <unordered_map>


namespace mimo {

    class Entity;
    class IQueueFactory;
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
            const IQueueFactory &factory,
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

        void end_run();

        void end_job();

        bool is_job_ended() const;

    private:

        unsigned int run;

        bool job_ended;

        std::unordered_map<std::string, std::unique_ptr<Queue>> queues;

        const std::unordered_map<std::string, unsigned int> &sync_groups;

        const IQueueFactory &factory;

        std::unordered_map<unsigned int, bool> get_group_status() const;

    };
}


#endif //MIMO_JOBOUTPUTS_H
