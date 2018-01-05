#ifndef MIMO_JOBMANAGER_H
#define MIMO_JOBMANAGER_H

#include <workflow/Step.h>
#include <workflow/Workflow.h>
#include "Job.h"


namespace mimo {

    /**
     * Manages the jobs being run by the system. Prevents too many jobs from being run.
     */
    class JobManager {
    public:
        JobManager(const workflow::Workflow &workflow_, unsigned int capacity);

        bool can_make_job(const std::shared_ptr<workflow::Step> &step) const;

        std::unique_ptr<Job> make_job(const std::shared_ptr<workflow::Step> &step);

        void destroy_job(const std::shared_ptr<workflow::Step> &step);

    private:

        const workflow::Workflow &workflow_;

        unsigned int capacity;

        std::unordered_map<unsigned int, unsigned int> counts;
    };
}


#endif //MIMO_JOBMANAGER_H
