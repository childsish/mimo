/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBMANAGER_H
#define MIMO_MOCKJOBMANAGER_H

#include <gmock/gmock.h>
#include "job_managers/IJobManager.h"


namespace mimo {

    class MockJobManager : public mimo::IJobManager {
    public:
        MOCK_METHOD2(add_entity, void(const std::shared_ptr<workflow::Input> &input, std::shared_ptr<Entity> entity));
        MOCK_CONST_METHOD0(has_runnable_job, bool());
        MOCK_METHOD0(get_runnable_job, std::shared_ptr<IJob>());
        MOCK_METHOD1(return_complete_job, void(std::shared_ptr<IJob>));

    };
}

#endif //MIMO_MOCKJOBMANAGER_H
