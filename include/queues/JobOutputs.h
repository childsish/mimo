/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBOUTPUTS_H
#define MIMO_JOBOUTPUTS_H

#include <workflow/Step.h>
#include "queues/Queue.h"
#include "queues/IJobOutputs.h"


namespace mimo {

    /** @brief A set of output queues from a step. */
    class JobOutputs : public IJobOutputs {
    public:

        JobOutputs(
            const workflow::OutputMap outputs,
            std::shared_ptr<IQueueFactory> factory
        );

        /** @brief Get named queue from outputs. */
        std::unique_ptr<IQueue> get_queue(const std::string &name) override;

        /** @brief Synchronise the named queues. */
        void synchronise_queues(const std::vector<std::string> &queues) override;

        /** @brief Get whether all queues can be pushed. */
        IJobOutputs::PushStatus get_status() const override;

        /**
         * @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full.
         */
        IJobOutputs::PushStatus get_status(const std::string &name) const override;

        /**
         * @brief Push an entity onto the specified queue.
         * If the queue is full or a synchronised queue is full, an exception is thrown.
         */
        void push(const std::string &name, std::shared_ptr<Entity> entity) override;

        /** @brief End the current run. */
        void end_run() override;

        /** @brief Close the job output. */
        void close() override;

        /** @brief Test if job output is closed. */
        bool is_closed() const override;

    private:

        unsigned int group_id;

        unsigned int run;

        bool job_ended;

        std::unordered_map<std::string, std::unique_ptr<IQueue>> queues;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::shared_ptr<IQueueFactory> factory;

        std::unordered_map<unsigned int, bool> get_group_status() const;

    };
}


#endif //MIMO_JOBOUTPUTS_H
