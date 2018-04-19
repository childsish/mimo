/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBDEPOTFACTORY_H
#define MIMO_MOCKJOBDEPOTFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "../../src/job_depots/ISingleJobDepotFactory.h"


namespace mimo {

    class MockSingleJobDepotFactory : public ISingleJobDepotFactory {
    public:
        std::unique_ptr<IJobDepot> make_depot(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override {
            return std::unique_ptr<IJobDepot>(this->make_depot_proxy(identifier));
        }

        MOCK_METHOD2(register_step, void(const std::shared_ptr<workflow::Step> &, std::shared_ptr<Step>));
        MOCK_CONST_METHOD1(make_depot_proxy, IJobDepot*(const std::shared_ptr<workflow::Step> &identifier));
    };
}

#endif //MIMO_MOCKJOBDEPOTFACTORY_H
