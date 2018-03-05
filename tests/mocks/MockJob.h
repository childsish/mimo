#ifndef MIMO_MOCKJOB_H
#define MIMO_MOCKJOB_H

#include <gmock/gmock.h>
#include "IJob.h"


namespace mimo {

    class MockJob : public IJob {
    public:
        MOCK_METHOD0(get_step_id, const std::shared_ptr<workflow::Step>());
        MOCK_METHOD0(get_job_id, unsigned int());
        MOCK_METHOD0(get_inputs, std::unique_ptr<IJobInputs>&());
        MOCK_METHOD0(get_outputs, std::unique_ptr<IJobOutputs>&());
        MOCK_CONST_METHOD0(can_run, bool());
        MOCK_METHOD0(run, void());
    };
}

#endif //MIMO_MOCKJOB_H
