#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include <mimo/Step.h>
#include "mocks/MockFactory.h"
#include "mocks/MockJobDepot.h"
#include "../src/depots/ISingleJobDepot.h"
#include "../src/depots/MultiJobDepot.h"


using ::testing::Return;

MOCK_FACTORY1(MockSingleJobDepot, mimo::ISingleJobDepot, std::shared_ptr<workflow::Step>)

TEST(JobDepotTest, test_correct_factory_is_used) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step1 = workflow->add_step("step1", {"input1", "input2"}, {});
    step1->synchronise_inputs({"input1", "input2"});
    auto step2 = workflow->add_step("step2", {}, {});

    auto sync_manager = new mimo::MockJobDepot();
    auto async_manager = new mimo::MockJobDepot();
    auto factory = std::make_shared<MockSingleJobDepotFactory>();
    EXPECT_CALL(*factory, make_depot_proxy(step1))
        .WillOnce(Return(sync_manager));
    EXPECT_CALL(*factory, make_depot_proxy(step2))
        .WillOnce(Return(async_manager));

    mimo::MultiJobDepot manager(workflow, factory);
}

TEST(JobDepotTest, test_empty_input_steps) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto factory = std::make_shared<MockSingleJobDepotFactory>();
    workflow->add_step("step", {}, {"output"});

    mimo::MultiJobDepot manager(workflow, factory);

    EXPECT_TRUE(manager.has_runnable_job());
}
