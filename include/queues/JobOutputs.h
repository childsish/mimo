/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBOUTPUTS_H
#define MIMO_JOBOUTPUTS_H

#include <workflow/Step.h>
#include "queues/Queue.h"
#include "queues/IJobOutputs.h"
#include "QueueFactory.h"


namespace mimo {

    class IQueue;

    /** @brief A set of output queues from a step. */
    class JobOutputs : public IJobOutputs {
    public:

        JobOutputs(
            const workflow::OutputMap &outputs,
            std::shared_ptr<IQueueFactory> factory
        );

        /** @brief Get whether all queues can be pushed. */
        IJobOutputs::PushStatus get_status() const override;

        /** @brief Get whether a queue can be pushed or if it, or a synchronised queue, is full. */
        IJobOutputs::PushStatus get_status(const std::string &name) const override;

        /** @brief Push an entity onto the specified queue.
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

        unsigned int run;
        bool job_ended;

        const workflow::OutputMap &outputs;
        std::unordered_map<std::string, std::unique_ptr<IQueue>> queues;
        std::shared_ptr<IQueueFactory> factory;

        std::unordered_map<unsigned int, bool> get_group_status() const;

    };
}


#endif //MIMO_JOBOUTPUTS_H
