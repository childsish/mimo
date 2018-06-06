/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBDEPOT_H
#define MIMO_JOBDEPOT_H

#include <queue>
#include <workflow/Workflow.h>
#include "IMultiJobDepot.h"
#include "SingleJobDepot.h"


namespace mimo {

    /** @brief: Manages the jobs being run by the system. Prevents too many jobs from being run. */
    class MultiJobDepot : public IMultiJobDepot {
    public:

        explicit MultiJobDepot(
            std::shared_ptr<workflow::Workflow> workflow_,
            std::shared_ptr<ISingleJobDepotFactory> factory = std::make_shared<SingleJobDepotFactory>(5)
        );

        void push(
            const std::shared_ptr<workflow::Input> &input_id,
            std::shared_ptr<Entity> entity
        ) override;

        bool can_queue(const std::shared_ptr<workflow::Output> &output_id) override;

        void queue_input(
            const std::shared_ptr<workflow::Output> &output_id,
            const IQueue &queue
        ) override;

        bool has_runnable_jobs() const override;

        std::set<std::unique_ptr<IJob>> get_runnable_jobs() override;

        void return_exhausted_job(std::unique_ptr<IJob> job) override;

    private:

        std::shared_ptr<workflow::Workflow> workflow_;
        std::unordered_map<std::shared_ptr<workflow::Step>, std::unique_ptr<ISingleJobDepot>> depots;
        std::set<std::shared_ptr<workflow::Step>> runnable_jobs;

    };
}

#endif //MIMO_JOBDEPOT_H
