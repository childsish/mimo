/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_MOCKSTEP_H
#define MIMO_MOCKSTEP_H

#include <gmock/gmock.h>

#include "mimo/Step.h"


namespace mimo {
    class MockStep : public mimo::Step {
    public:
        MOCK_METHOD2(run, bool(mimo::Inputs&, mimo::Outputs&));
    };
}

#endif //MIMO_MOCKSTEP_H
