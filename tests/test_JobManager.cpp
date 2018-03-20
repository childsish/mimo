#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include "mocks/MockJobManager.h"
#include "mocks/MockJobManagerFactory.h"
#include "JobManager.h"
#include "Step.h"


using ::testing::Return;

TEST(JobManagerTest, test_correct_factory_is_used) {
    workflow::Workflow workflow;
    auto step1 = workflow.add_step("step1", {"input1", "input2"}, {});
    step1->synchronise_inputs({"input1", "input2"});
    auto step2 = workflow.add_step("step2", {}, {});

    auto sync_manager = new mimo::MockJobManager();
    auto async_manager = new mimo::MockJobManager();
    auto factory = std::make_shared<mimo::MockJobManagerFactory>();
    EXPECT_CALL(*factory, make_manager_proxy(step1))
        .WillOnce(Return(sync_manager));
    EXPECT_CALL(*factory, make_manager_proxy(step2))
        .WillOnce(Return(async_manager));

    mimo::JobManager manager(workflow, factory);
}
