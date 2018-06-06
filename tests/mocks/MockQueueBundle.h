/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKQUEUEBUNDLE_H
#define MIMO_MOCKQUEUEBUNDLE_H

#include <gmock/gmock.h>
#include "../../src/queues/IQueueBundle.h"


namespace mimo {
    class MockQueueBundle : public IQueueBundle {
    public:
        void acquire_queue(const workflow::Connection &connection, std::unique_ptr<IQueue> queue) {
            this->acquire_queue_proxy(connection, queue.get());
        }


        MOCK_CONST_METHOD0(get_pop_status, PopStatus());
        MOCK_CONST_METHOD1(get_pop_status, PopStatus(const std::string &name));
        MOCK_METHOD1(peek, std::shared_ptr<Entity>(const std::string &name));
        MOCK_METHOD1(pop, std::shared_ptr<Entity>(const std::string &));
        MOCK_CONST_METHOD0(get_push_status, PushStatus());
        MOCK_CONST_METHOD1(get_push_status, PushStatus(const std::string &));
        MOCK_METHOD2(push, void(const std::string &, std::shared_ptr<Entity>));
        MOCK_CONST_METHOD0(get_identifiers, const ConnectionMap &());
        MOCK_METHOD1(release_queue, std::unique_ptr<IQueue>(const workflow::Connection &));
        MOCK_METHOD2(acquire_queue_proxy, void(const workflow::Connection &, IQueue *));
        MOCK_METHOD2(push, void(const std::string &, const IQueue &));
    };
}

#endif //MIMO_MOCKQUEUEBUNDLE_H
