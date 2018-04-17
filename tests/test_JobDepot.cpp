#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include "MockJobDepot.h"
#include "MockSingleJobDepotFactory.h"
#include "../src/job_depots/JobDepot.h"
#include "mimo/Step.h"


using ::testing::Return;

TEST(JobManagerTest, test_correct_factory_is_used) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step1 = workflow->add_step("step1", {"input1", "input2"}, {});
    step1->synchronise_inputs({"input1", "input2"});
    auto step2 = workflow->add_step("step2", {}, {});

    auto sync_manager = new mimo::MockJobDepot();
    auto async_manager = new mimo::MockJobDepot();
    auto factory = std::make_shared<mimo::MockSingleJobDepotFactory>();
    EXPECT_CALL(*factory, make_manager_proxy(step1))
        .WillOnce(Return(sync_manager));
    EXPECT_CALL(*factory, make_manager_proxy(step2))
        .WillOnce(Return(async_manager));

    mimo::JobDepot manager(workflow, factory);
}
