/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBINPUT_H
#define MIMO_JOBINPUT_H

#include "queues/IJobInputs.h"


namespace mimo {

    class Entity;
    class IQueue;

    /**
     * @brief: A set of input queues to a step.
     */
    class JobInputs : public IJobInputs {
    public:

        JobInputs();

        /**
         * @brief Add a queue to the inputs.
         * @param name Name of the queue.
         * @param queue The queue.
         */
        void add_queue(const std::string &name, std::unique_ptr<IQueue> queue) override;

        /**
         * @brief Synchronise the named queues.
         */
        void synchronise_queues(const std::vector<std::string> &queues) override;

        /**
         * Get whether all queues can be popped.
         * @return
         */
        IJobInputs::PopStatus get_status() const override;

        /**
         * @brief Get whether a queue can be popped or if it, or a synchronised queue, is empty.
         * @param name Queue to query.
         */
        IJobInputs::PopStatus get_status(const std::string &name) const override;

        /**
         * @brief Peek at the first item in the named queue but do not pop it.
         */
        std::shared_ptr<Entity> peek(const std::string &name) override;

        /**
         * @brief Pop and return the first item of the named queue.
         */
        std::shared_ptr<Entity> pop(const std::string &name) override;

        /**
         * @brief Get whether all queues are closed.
         */
        bool is_closed() const override;

    private:

        unsigned int group_id;

        std::unordered_map<std::string, std::unique_ptr<IQueue>> queues;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<unsigned int, bool> get_group_status() const;
    };
}


#endif //MIMO_JOBINPUT_H
