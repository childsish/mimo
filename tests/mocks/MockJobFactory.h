/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBFACTORY_H
#define MIMO_MOCKJOBFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "IFactory.h"
#include "JobFactory.h"


namespace mimo {

    class Job;

    class MockJobFactory : public IJobFactory {
    public:
        IJob *make_raw(const std::shared_ptr<workflow::Step> &step_id, std::shared_ptr<Step> step) const override {
            return this->make_raw_proxy(step_id, step);
        }

        std::shared_ptr<IJob> make_shared(const std::shared_ptr<workflow::Step> &step_id, std::shared_ptr<Step> step) const override {
            return std::shared_ptr<IJob>(this->make_shared_proxy(step_id, step));
        }

        std::unique_ptr<IJob> make_unique(const std::shared_ptr<workflow::Step> &step_id, std::shared_ptr<Step> step) const override {
            return std::unique_ptr<IJob>(this->make_unique_proxy(step_id, step));
        }

        MOCK_CONST_METHOD2(make_raw_proxy, IJob*(const std::shared_ptr<workflow::Step> &, std::shared_ptr<Step>));
        MOCK_CONST_METHOD2(make_shared_proxy, IJob*(const std::shared_ptr<workflow::Step> &, std::shared_ptr<Step>));
        MOCK_CONST_METHOD2(make_unique_proxy, IJob*(const std::shared_ptr<workflow::Step> &, std::shared_ptr<Step>));
    };
}

#endif //MIMO_MOCKJOBINPUTSFACTORY_H
