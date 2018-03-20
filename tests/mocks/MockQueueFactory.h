/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKQUEUEFACTORY_H
#define MIMO_MOCKQUEUEFACTORY_H

#include <gmock/gmock.h>
#include "QueueFactory.h"

namespace mimo {

    class MockQueueFactory : public IQueueFactory {
    public:
        MOCK_CONST_METHOD0(make_raw, IQueue*());
    };
}


#endif //MIMO_MOCKQUEUEFACTORY_H
