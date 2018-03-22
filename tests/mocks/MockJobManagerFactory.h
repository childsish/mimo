/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBMANAGERFACTORY_H
#define MIMO_MOCKJOBMANAGERFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "IJobManagerFactory.h"


namespace mimo {

    class MockJobManagerFactory : public IJobManagerFactory {
    public:
        std::unique_ptr<IJobManager> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const override {
            return std::unique_ptr<IJobManager>(this->make_manager_proxy(identifier));
        }

        MOCK_CONST_METHOD1(make_manager_proxy, IJobManager*(const std::shared_ptr<workflow::Step> &identifier));
    };
}

#endif //MIMO_MOCKJOBMANAGERFACTORY_H