/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBDEPOTFACTORY_H
#define MIMO_MOCKJOBDEPOTFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "job_ports/ISingleJobDepotFactory.h"


namespace mimo {

    class MockSingleJobDepotFactory : public ISingleJobDepotFactory {
    public:
        std::unique_ptr<IJobDepot> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override {
            return std::unique_ptr<IJobDepot>(this->make_manager_proxy(identifier));
        }

        MOCK_METHOD2(register_step, void(const std::shared_ptr<workflow::Step> &, StepConstructor));
        MOCK_CONST_METHOD1(make_manager_proxy, IJobDepot*(const std::shared_ptr<workflow::Step> &identifier));
    };
}

#endif //MIMO_MOCKJOBDEPOTFACTORY_H
