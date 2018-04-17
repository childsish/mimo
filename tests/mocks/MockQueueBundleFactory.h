/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKQUEUEBUNDLEFACTORY_H
#define MIMO_MOCKQUEUEBUNDLEFACTORY_H

#include "../../src/queues/IQueueBundleFactory.h"


namespace mimo {
    class MockQueueBundleFactory : public IQueueBundleFactory {
    public:
        MOCK_CONST_METHOD1(make_raw, IQueueBundle *(const workflow::InputMap &));
        MOCK_CONST_METHOD1(make_shared, std::shared_ptr<IQueueBundle>(const workflow::InputMap &));
        MOCK_CONST_METHOD1(make_unique, std::unique_ptr<IQueueBundle>(const workflow::InputMap &));
        MOCK_CONST_METHOD1(make_raw, IQueueBundle *(const workflow::OutputMap &));
        MOCK_CONST_METHOD1(make_shared, std::shared_ptr<IQueueBundle>(const workflow::OutputMap &));
        MOCK_CONST_METHOD1(make_unique, std::unique_ptr<IQueueBundle>(const workflow::OutputMap &));
    };
}

#endif //MIMO_MOCKQUEUEBUNDLEFACTORY_H
