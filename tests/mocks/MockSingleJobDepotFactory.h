/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKSINGLEJOBDEPOTFACTORY_H
#define MIMO_MOCKSINGLEJOBDEPOTFACTORY_H

#include <gmock/gmock.h>
#include "../../src/depots/SingleJobDepotFactory.h"


namespace mimo {
    class MockSingleJobDepotFactory : public ISingleJobDepotFactory {
    public:
        MOCK_METHOD2(register_step, void(std::shared_ptr<workflow::Step>, std::shared_ptr<Step>));
        MOCK_METHOD1(make_raw, ISingleJobDepot *(std::shared_ptr<workflow::Step>));
    };
}

#endif //MIMO_MOCKSINGLEJOBDEPOTFACTORY_H
