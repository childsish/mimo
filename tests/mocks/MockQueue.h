/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_MOCKQUEUE_H
#define MIMO_MOCKQUEUE_H

#include <gmock/gmock.h>
#include "mimo/Entity.h"
#include "../../src/Factory.h"

#include "../../src/queues/IQueue.h"


namespace mimo {
    class MockQueue : public IQueue {
    public:
        MOCK_CONST_METHOD0(can_pop, bool());
        MOCK_METHOD0(peek, std::shared_ptr<Entity>());
        MOCK_METHOD0(pop, std::shared_ptr<Entity>());
        MOCK_CONST_METHOD0(can_push, bool());
        MOCK_METHOD1(push, void(std::shared_ptr<Entity> entity));
        MOCK_METHOD1(push, void(const IQueue &queue));
        MOCK_METHOD0(close, void());
        MOCK_CONST_METHOD0(is_closed, bool());
        MOCK_METHOD0(begin, IForwardIterator<std::shared_ptr<Entity>>());
        MOCK_CONST_METHOD0(begin, IForwardIterator<std::shared_ptr<Entity>>());
        MOCK_METHOD0(end, IForwardIterator<std::shared_ptr<Entity>>());
        MOCK_CONST_METHOD0(end, IForwardIterator<std::shared_ptr<Entity>>());
    };
}



#endif //MIMO_MOCKQUEUE_H
