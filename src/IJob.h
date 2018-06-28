/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOB_H
#define MIMO_IJOB_H

#include <memory>
#include <workflow/Step.h>


namespace mimo {

    class IQueueBundle;

    /** @brief A manager of a step's input, output and readiness. */
    class IJob {
    public:
        virtual ~IJob() = default;

        /** @brief Gets the identifier for the step this job executes. */
        virtual const std::shared_ptr<workflow::Step> get_step_id() const = 0;

        /** @brief Gets the identifier for the job. */
        virtual unsigned int get_job_id() const = 0;

        /** @brief Transfer all the entities from one input queue bundle to another. */
        virtual void transfer_input(IQueueBundle &bundle) = 0;

        /** @brief Gets the inputs for the job. */
        virtual std::shared_ptr<IQueueBundle> get_inputs() = 0;

        /** @brief Gets the outputs for the job. */
        virtual std::shared_ptr<IQueueBundle> get_outputs() = 0;

        /** @brief Check if the job can be run. */
        virtual bool can_run() const = 0;

        /** @brief Runs the job. */
        virtual void run() = 0;

        /** @brief True if job is complete. */
        virtual bool is_complete() const = 0;
    };
}

#endif //MIMO_IJOB_H
