/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include <memory>


namespace workflow {
    class Step;
}

namespace mimo {

    class JobInputs;
    class JobOutputs;
    class Step;

    /**
     * @brief:
     */
    class Job {
    public:

        const std::shared_ptr<workflow::Step> identifier;

        explicit Job(
            const std::shared_ptr<workflow::Step> identifier,
            std::unique_ptr<Step> step,
            std::unique_ptr<JobInputs> inputs,
            std::unique_ptr<JobOutputs> outputs
        );

        std::unique_ptr<JobInputs> &ins();

        std::unique_ptr<JobOutputs> &outs();

        void run();

        bool is_complete() const;

    private:

        bool completed;

        std::unique_ptr<mimo::Step> step;

        std::unique_ptr<JobInputs> inputs;
        std::unique_ptr<JobOutputs> outputs;
    };
}

#endif //MIMO_JOB_H
