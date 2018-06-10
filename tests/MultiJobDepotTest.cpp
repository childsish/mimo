#include <gtest/gtest.h>
#include <workflow/Workflow.h>
#include <mimo/Entity.h>
#include "mocks/MockSingleJobDepot.h"
#include "mocks/MockSingleJobDepotFactory.h"
#include "../src/depots/MultiJobDepot.h"


using ::testing::Return;

TEST(MultiJobDepotTest, test_construct_with_runnable_jobs) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step_id = workflow->add_step("step1", {}, {});

    auto single_job_depot = new mimo::MockSingleJobDepot();
    EXPECT_CALL(*single_job_depot, has_runnable_jobs())
        .WillRepeatedly(Return(true));
    auto factory = std::make_shared<mimo::MockSingleJobDepotFactory>();
    EXPECT_CALL(*factory, make_raw(step_id))
        .WillOnce(Return(single_job_depot));

    mimo::MultiJobDepot multi_job_depot(workflow, factory);
    EXPECT_TRUE(multi_job_depot.has_runnable_jobs());
    EXPECT_GT(multi_job_depot.get_runnable_jobs().size(), 0);
    EXPECT_FALSE(multi_job_depot.has_runnable_jobs());
}

TEST(MultiJobDepotTest, test_push_with_runnable_jobs) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step_id = workflow->add_step("step1", {"input"}, {});
    auto input_id = step_id->get_inputs()->at("input");
    auto entity = std::make_shared<mimo::Entity>();

    auto single_job_depot = new mimo::MockSingleJobDepot();
    EXPECT_CALL(*single_job_depot, has_runnable_jobs())
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    auto factory = std::make_shared<mimo::MockSingleJobDepotFactory>();
    EXPECT_CALL(*factory, make_raw(step_id))
        .WillOnce(Return(single_job_depot));

    mimo::MultiJobDepot multi_job_depot(workflow, factory);
    EXPECT_FALSE(multi_job_depot.has_runnable_jobs());
    multi_job_depot.push(dynamic_cast<workflow::Input&>(*input_id), entity);
    EXPECT_TRUE(multi_job_depot.has_runnable_jobs());
    EXPECT_GT(multi_job_depot.get_runnable_jobs().size(), 0);
    EXPECT_FALSE(multi_job_depot.has_runnable_jobs());
}

TEST(MultiJobDepotTest, test_push_without_runnable_jobs) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step_id = workflow->add_step("step1", {"input"}, {});
    auto input_id = step_id->get_inputs()->at("input");
    auto entity = std::make_shared<mimo::Entity>();

    auto single_job_depot = new mimo::MockSingleJobDepot();
    EXPECT_CALL(*single_job_depot, has_runnable_jobs())
        .WillRepeatedly(Return(false));
    auto factory = std::make_shared<mimo::MockSingleJobDepotFactory>();
    EXPECT_CALL(*factory, make_raw(step_id))
        .WillOnce(Return(single_job_depot));

    mimo::MultiJobDepot multi_job_depot(workflow, factory);
    EXPECT_FALSE(multi_job_depot.has_runnable_jobs());
    multi_job_depot.push(dynamic_cast<workflow::Input&>(*input_id), entity);
    EXPECT_FALSE(multi_job_depot.has_runnable_jobs());
}

TEST(MultiJobDepotTest, test_can_queue) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto parent_step = workflow->add_step("parent_step", {}, {"output"});
    auto output_id = parent_step->get_outputs()->at("output");
    auto step_id1 = workflow->add_step("step1", {"input1"}, {});
    auto step_id2 = workflow->add_step("step2", {"input2"}, {});
    parent_step->pipe(step_id1);
    parent_step->pipe(step_id2);
    auto input_id1 = step_id1->get_inputs()->at("input1");
    auto input_id2 = step_id2->get_inputs()->at("input2");
    auto entity = std::make_shared<mimo::Entity>();

    auto parent_depot = new mimo::MockSingleJobDepot();
    auto depot1 = new mimo::MockSingleJobDepot();
    EXPECT_CALL(*depot1, can_queue(dynamic_cast<workflow::Input&>(*input_id1)))
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillOnce(Return(true))
        .WillOnce(Return(true));
    auto depot2 = new mimo::MockSingleJobDepot();
    EXPECT_CALL(*depot2, can_queue(dynamic_cast<workflow::Input&>(*input_id2)))
        .WillOnce(Return(false))
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    auto factory = std::make_shared<mimo::MockSingleJobDepotFactory>();
    EXPECT_CALL(*factory, make_raw(parent_step))
        .WillOnce(Return(parent_depot));
    EXPECT_CALL(*factory, make_raw(step_id1))
        .WillOnce(Return(depot1));
    EXPECT_CALL(*factory, make_raw(step_id2))
        .WillOnce(Return(depot2));

    mimo::MultiJobDepot multi_job_depot(workflow, factory);
    EXPECT_FALSE(multi_job_depot.can_queue(dynamic_cast<workflow::Output&>(*output_id)));
    EXPECT_FALSE(multi_job_depot.can_queue(dynamic_cast<workflow::Output&>(*output_id)));
    EXPECT_FALSE(multi_job_depot.can_queue(dynamic_cast<workflow::Output&>(*output_id)));
    EXPECT_TRUE(multi_job_depot.can_queue(dynamic_cast<workflow::Output&>(*output_id)));
}
