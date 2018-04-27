/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_ASYNCHRONOUSJOBDEPOT_H
#define MIMO_ASYNCHRONOUSJOBDEPOT_H

#include <workflow/Step.h>
#include "IJobDepot.h"
#include "../JobFactory.h"


namespace mimo {

    class Step;

    class AsynchronousJobDepot : public IJobDepot {
    public:

        AsynchronousJobDepot(const std::shared_ptr<workflow::Step> &identifier,
                               std::shared_ptr<Step> step,
                               std::shared_ptr<IJobFactory> job_factory = std::make_shared<JobFactory>());

        void add_entity(const std::shared_ptr<workflow::Input> &input,
                        std::shared_ptr<Entity> entity) override;

        bool has_runnable_job() const override;

        std::shared_ptr<IJob> get_runnable_job() override;

        void return_job(std::shared_ptr<IJob> job) override;

    private:

        bool available;
        const std::shared_ptr<workflow::Step> &identifier;
        std::shared_ptr<Step> step;
        std::shared_ptr<IJob> job;

    };
}


#endif //MIMO_ASYNCHRONOUSJOBDEPOT_H
