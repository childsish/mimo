/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKQUEUEFACTORY_H
#define MIMO_MOCKQUEUEFACTORY_H

#include <gmock/gmock.h>
#include "QueueFactory.h"

namespace mimo {

    class MockQueueFactory : public IQueueFactory {
    public:
        IQueue *make_raw() const override {
            return this->make_raw_proxy();
        }

        std::shared_ptr<IQueue> make_shared() const override {
            return std::shared_ptr<IQueue>(this->make_shared_proxy());
        }

        std::unique_ptr<IQueue> make_unique() const override {
            return std::unique_ptr<IQueue>(this->make_unique_proxy());
        }

        MOCK_CONST_METHOD0(make_raw_proxy, IQueue*());
        MOCK_CONST_METHOD0(make_shared_proxy, IQueue*());
        MOCK_CONST_METHOD0(make_unique_proxy, IQueue*());
    };
}


#endif //MIMO_MOCKQUEUEFACTORY_H
