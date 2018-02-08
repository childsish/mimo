/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_MOCKJOBOUTPUTS_H
#define MIMO_MOCKJOBOUTPUTS_H

#include <gmock/gmock.h>

#include "queues/IJobOutputs.h"


class MockJobOutputs : public mimo::IJobOutputs {
public:
    virtual std::unique_ptr<mimo::IQueue> get_queue(const std::string &name) {
        return std::unique_ptr<mimo::IQueue>(this->get_queue_proxy(name));
    }

    MOCK_METHOD1(get_queue_proxy, mimo::IQueue*(const std::string&));
    MOCK_METHOD1(synchronise_queues, void(const std::vector<std::string>&));
    MOCK_CONST_METHOD0(get_status, PushStatus());
    MOCK_CONST_METHOD1(get_status, PushStatus(const std::string&));
    MOCK_METHOD2(push, void(const std::string&, std::shared_ptr<mimo::Entity>));
    MOCK_METHOD0(end_run, void());
    MOCK_METHOD0(close, void());
    MOCK_CONST_METHOD0(is_closed, bool());
};

#endif //MIMO_MOCKJOBOUTPUTS_H
