/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBMANAGER_H
#define MIMO_JOBMANAGER_H

#include <queue>
#include <workflow/Workflow.h>
#include "IJobManager.h"
#include "JobManagerFactory.h"


namespace mimo {

    class Step;

    /** @brief: Manages the jobs being run by the system. Prevents too many jobs from being run. */
    class JobManager : public IJobManager {
    public:

        explicit JobManager(const workflow::Workflow &workflow_,
                            std::shared_ptr<IJobManagerFactory> factory = std::make_shared<JobManagerFactory>(5));

        void add_entity(const std::shared_ptr<workflow::Input> &input,
                        std::shared_ptr<Entity> entity) override;

        void add_entity(const std::shared_ptr<workflow::Output> &identifier,
                        std::shared_ptr<Entity> entity);

        bool has_runnable_job() const override;

        std::shared_ptr<IJob> get_runnable_job() override;

        void return_complete_job(std::shared_ptr<IJob> job) override;

    private:

        const workflow::Workflow &workflow_;

        std::unordered_map<std::shared_ptr<workflow::Step>, std::unique_ptr<IJobManager>> jobs;

        std::queue<std::shared_ptr<workflow::Step>> runnable_jobs;

    };
}

#endif //MIMO_JOBMANAGER_H
