/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBINPUTSFACTORY_H
#define MIMO_MOCKJOBINPUTSFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "IFactory.h"
#include "JobInputFactory.h"


namespace mimo {

    class JobInputs;

    class MockJobInputsFactory : public IJobInputsFactory {
    public:
        MOCK_CONST_METHOD1(make_raw, IJobInputs*(const workflow::InputMap &));
    };
}

#endif //MIMO_MOCKJOBINPUTSFACTORY_H
