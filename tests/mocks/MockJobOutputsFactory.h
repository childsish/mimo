/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBOUTPUTSFACTORY_H
#define MIMO_MOCKJOBOUTPUTSFACTORY_H

#include <gmock/gmock.h>
#include "IFactory.h"
#include "JobOutputFactory.h"


namespace mimo {

    class JobOutputs;

    class MockJobOutputsFactory : public IJobOutputsFactory {
    public:
        MOCK_CONST_METHOD1(make_raw, IJobOutputs*(const workflow::OutputMap &));
    };
}

#endif //MIMO_MOCKJOBOUTPUTSFACTORY_H
