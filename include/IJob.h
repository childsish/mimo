/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_IJOB_H
#define MIMO_IJOB_H

#include <memory>


namespace workflow {
    class Step;
}

namespace mimo {

    class IJobInputs;
    class IJobOutputs;

    /** @brief */
    class IJob {
    public:
        virtual ~IJob() = default;

        /** @brief Gets the identifier for the step this job executes. */
        virtual const std::shared_ptr<workflow::Step> get_step_id() const = 0;

        /** @brief Gets the identifier for the job. */
        virtual unsigned int get_job_id() const = 0;

        /** @brief Gets the inputs for the job. */
        virtual std::unique_ptr<IJobInputs> &get_inputs() = 0;

        /** @brief Gets the outputs for the job. */
        virtual std::unique_ptr<IJobOutputs> &get_outputs() = 0;

        /** @brief Check if the job can be run. */
        virtual bool can_run() const = 0;

        /** @brief Runs the job. */
        virtual void run() = 0;
    };
}

#endif //MIMO_IJOB_H
