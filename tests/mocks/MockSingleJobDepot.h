/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBDEPOT_H
#define MIMO_MOCKJOBDEPOT_H

#include <gmock/gmock.h>
#include "../../src/IJob.h"
#include "../../src/depots/ISingleJobDepot.h"


namespace mimo {

    class MockSingleJobDepot : public mimo::ISingleJobDepot {
    public:
        void return_exhausted_job(std::unique_ptr<IJob> job) override {
            this->return_exhausted_job_proxy(job.get());
        }

        MOCK_METHOD1(can_queue, bool(const workflow::Input &input_id));
        MOCK_METHOD2(queue_input, void(const workflow::Input &input_id, const IQueue &queue));

        MOCK_METHOD2(push, void(const workflow::Input &input_id, std::shared_ptr<Entity> entity));
        MOCK_CONST_METHOD0(has_runnable_jobs, bool());
        MOCK_METHOD0(get_runnable_jobs, std::set<std::unique_ptr<IJob>, JobComparator>());
        MOCK_METHOD1(return_exhausted_job_proxy, void(IJob *job));
    };
}

#endif //MIMO_MOCKJOBMANAGER_H
