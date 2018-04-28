/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_SYNCHRONOUSJOBDEPOT_H
#define MIMO_SYNCHRONOUSJOBDEPOT_H

#include <list>
#include <workflow/Step.h>
#include "IJobDepot.h"
#include "../JobFactory.h"


namespace mimo {

    class Step;

    /**
     * @brief Storage for a synchronous step.
     * The inputs for a synchronous step are all synchronised to each other and the output for a
     * synchronous step are all synchronised to each other. This means that the step can have
     * several jobs that run in parallel. This class manages these jobs when they are idle.
     * @invariant There is always at least one job in the queue, although it may not be runnable.
     **/
    class SynchronousJobDepot : public IJobDepot {
    public:

        SynchronousJobDepot(unsigned int capacity,
                              const std::shared_ptr<workflow::Step> &identifier,
                              std::shared_ptr<Step> step,
                              std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>());

        void add_entity(const std::shared_ptr<workflow::Input> &input,
                        std::shared_ptr<Entity> entity) override;

        bool has_runnable_job() const override;

        std::shared_ptr<IJob> get_runnable_job() override;

        void return_job(std::shared_ptr<IJob> job) override;

    private:

        unsigned int active_jobs;
        unsigned int capacity;
        std::shared_ptr<workflow::Step> identifier;
        std::shared_ptr<Step> step;
        std::shared_ptr<IJobFactory> job_factory;
        std::list<std::shared_ptr<IJob>> jobs;

        void make_job();

    };
}


#endif //MIMO_SYNCHRONOUSJOBDEPOT_H
