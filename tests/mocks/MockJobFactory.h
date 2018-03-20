/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBFACTORY_H
#define MIMO_MOCKJOBFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "IFactory.h"
#include "JobFactory.h"


namespace mimo {

    class MockJobFactory : public IJobFactory {
    public:
        MOCK_CONST_METHOD2(make_raw, IJob*(const std::shared_ptr<workflow::Step> &, std::shared_ptr<Step>));
    };
}

#endif //MIMO_MOCKJOBINPUTSFACTORY_H
