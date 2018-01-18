/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBOUTPUTS_H
#define MIMO_JOBOUTPUTS_H

#include "queues/IJobOutputs.h"


namespace mimo {

    /**
     * @brief A set of output queues from a step.
     */
    class JobOutputs : public IJobOutputs {
    public:

        JobOutputs(IQueueFactory &factory, const std::vector<std::string> &sync_groups);

        /**
         * @brief Get named queue from outputs.
         */
        std::unique_ptr<IQueue> get_queue(const std::string &name);

        /**
         * @brief Synchronise the named queues.
         */
        void synchronise_queues(const std::vector<std::string> &queues);

        /**
         * @brief Get whether all queues can be pushed.
         */
        IJobOutputs::PushStatus get_status() const;

        /**
         * @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full.
         * @param name Queue to query.
         */
        IJobOutputs::PushStatus get_status(const std::string &name) const;

        /**
         * @brief Push an entity onto the specified queue.
         * If the queue is full or a synchronised queue is full, an exception is thrown.
         * @param name
         * @param entity
         */
        void push(const std::string &name, std::shared_ptr<Entity> entity);

        void end_run();

        void close();

        bool is_closed() const;

    private:

        unsigned int group_id;

        unsigned int run;

        bool job_ended;

        std::unordered_map<std::string, std::unique_ptr<IQueue>> queues;

        std::unordered_map<std::string, unsigned int> sync_groups;

        IQueueFactory &factory;

        std::unordered_map<unsigned int, bool> get_group_status() const;

    };
}


#endif //MIMO_JOBOUTPUTS_H
