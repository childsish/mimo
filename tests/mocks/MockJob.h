#ifndef MIMO_MOCKJOB_H
#define MIMO_MOCKJOB_H

#include <gmock/gmock.h>
#include "../../src/IJob.h"


namespace mimo {

    class MockJob : public IJob {
    public:
        MOCK_CONST_METHOD0(get_step_id, const std::shared_ptr<workflow::Step>());
        MOCK_CONST_METHOD0(get_job_id, unsigned int());
        MOCK_METHOD0(get_inputs, std::shared_ptr<IQueueBundle>&());
        MOCK_METHOD0(get_outputs, std::shared_ptr<IQueueBundle>&());
        MOCK_CONST_METHOD0(can_run, bool());
        MOCK_METHOD0(run, void());
        MOCK_CONST_METHOD0(is_complete, bool());
    };
}

#endif //MIMO_MOCKJOB_H
