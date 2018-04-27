/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBDEPOT_H
#define MIMO_JOBDEPOT_H

#include <queue>
#include <workflow/Workflow.h>
#include "IMultiJobDepot.h"
#include "SingleJobDepotFactory.h"


namespace mimo {

    /** @brief: Manages the jobs being run by the system. Prevents too many jobs from being run. */
    class JobDepot : public IMultiJobDepot {
    public:

        explicit JobDepot(
            std::shared_ptr<workflow::Workflow> workflow_,
            std::shared_ptr<ISingleJobDepotFactory> factory = std::make_shared<SingleJobDepotFactory>(5)
        );

        void add_entity(
            const std::shared_ptr<workflow::Input> &input,
            std::shared_ptr<Entity> entity
        ) override;

        void add_entity(
            const std::shared_ptr<workflow::Output> &identifier,
            std::shared_ptr<Entity> entity
        ) override;

        bool has_runnable_job() const override;

        std::shared_ptr<IJob> get_runnable_job() override;

        void return_job(std::shared_ptr<IJob> job) override;

    private:

        std::shared_ptr<workflow::Workflow> workflow_;

        std::unordered_map<std::shared_ptr<workflow::Step>, std::unique_ptr<IJobDepot>> jobs;

        std::queue<std::shared_ptr<workflow::Step>> runnable_jobs;
        std::unordered_set<std::shared_ptr<workflow::Step>> queued_runnable_jobs;

    };
}

#endif //MIMO_JOBDEPOT_H
