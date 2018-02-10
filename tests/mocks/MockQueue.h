/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_MOCKQUEUE_H
#define MIMO_MOCKQUEUE_H

#include <gmock/gmock.h>
#include "Entity.h"
#include "Factory.h"

#include "queues/IQueue.h"


namespace mimo {
    class MockQueue : public IQueue {
    public:
        MOCK_METHOD0(peek, std::shared_ptr<Entity>());
        MOCK_METHOD0(pop, std::shared_ptr<Entity>());
        MOCK_METHOD1(push, void(std::shared_ptr<Entity> entity));
        MOCK_CONST_METHOD0(can_pop, bool());
        MOCK_CONST_METHOD0(can_push, bool());
        MOCK_CONST_METHOD0(is_closed, bool());
        MOCK_CONST_METHOD0(is_empty, bool());
        MOCK_CONST_METHOD0(is_full, bool());
    };


    class MockQueueFactory : public IFactory<IQueue> {
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



#endif //MIMO_MOCKQUEUE_H
