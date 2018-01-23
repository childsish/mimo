#ifndef MIMO_JOBMANAGER_H
#define MIMO_JOBMANAGER_H

#include <queue>
#include <workflow/Step.h>
#include <workflow/Workflow.h>
#include "IJobManager.h"


namespace mimo {

    class Step;

    /**
     * @brief: Manages the jobs being run by the system. Prevents too many jobs from being run.
     */
    class JobManager : public IJobManager {
    public:

        JobManager(const workflow::Workflow &workflow_, unsigned int capacity);

        void add_entity(const std::shared_ptr<workflow::Input> identifier,
                        std::shared_ptr<Entity> entity) override;

        void add_entity(const std::shared_ptr<workflow::Output> identifier,
                        std::shared_ptr<Entity> entity) override;

        bool has_job() const override;

        std::unique_ptr<Job> get_job() override;

    private:

        const workflow::Workflow &workflow_;

        unsigned int capacity;

        std::unordered_map<unsigned int, std::shared_ptr<Step>> counts;

        std::unordered_map<unsigned int, std::queue<std::shared_ptr<Entity>>> inputs;
    };
}


#endif //MIMO_JOBMANAGER_H
