/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKSTEP_H
#define MIMO_MOCKSTEP_H

#include <gmock/gmock.h>

#include "mimo/Step.h"


namespace mimo {
    class MockStep : public mimo::Step {
    public:
        MOCK_CONST_METHOD0(get_name, const std::string &());
        MOCK_CONST_METHOD0(get_inputs, const std::vector<std::string> &());
        MOCK_CONST_METHOD0(get_outputs, const std::vector<std::string> &());
        MOCK_METHOD2(run, bool(mimo::IInputs&, mimo::IOutputs&));
    };
}

#endif //MIMO_MOCKSTEP_H
