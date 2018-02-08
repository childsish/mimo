/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_MOCKJOBINPUTS_H
#define MIMO_MOCKJOBINPUTS_H

#include <gmock/gmock.h>

#include "queues/IJobInputs.h"


class MockJobInputs : public mimo::IJobInputs {
public:
    virtual void add_queue(const std::string &name, std::unique_ptr<mimo::IQueue> queue) {
        this->add_queue_proxy(name, queue.get());
    }

    MOCK_METHOD2(add_queue_proxy, void(const std::string&, mimo::IQueue*));
    MOCK_METHOD1(synchronise_queues, void(const std::vector<std::string>&));
    MOCK_CONST_METHOD0(get_status, PopStatus());
    MOCK_CONST_METHOD1(get_status, PopStatus(const std::string&));
    MOCK_METHOD1(peek, std::shared_ptr<mimo::Entity>(const std::string&));
    MOCK_METHOD1(pop, std::shared_ptr<mimo::Entity>(const std::string&));
    MOCK_CONST_METHOD0(is_empty, bool());
    MOCK_CONST_METHOD0(is_closed, bool());
};

#endif //MIMO_MOCKJOBINPUTS_H
