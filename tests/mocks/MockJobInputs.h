#ifndef MIMO_MOCKJOBINPUTS_H
#define MIMO_MOCKJOBINPUTS_H

#include <gmock/gmock.h>

#include "queues/IJobInputs.h"


namespace mimo {

    class MockJobInputs : public mimo::IJobInputs {
    public:
        MOCK_METHOD2(push, void(const std::string&, std::shared_ptr<Entity>));
        MOCK_CONST_METHOD0(get_status, PopStatus());
        MOCK_CONST_METHOD1(get_status, PopStatus(const std::string&));
        MOCK_METHOD1(peek, std::shared_ptr<mimo::Entity>(const std::string&));
        MOCK_METHOD1(pop, std::shared_ptr<mimo::Entity>(const std::string&));
        MOCK_CONST_METHOD0(is_empty, bool());
        MOCK_CONST_METHOD0(is_closed, bool());
    };
}

#endif //MIMO_MOCKJOBINPUTS_H
