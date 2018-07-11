#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Workflow.h>
#include "mocks/MockFactory.h"
#include "mocks/MockJob.h"
#include "mocks/MockStep.h"
#include "mocks/MockQueue.h"
#include "mocks/MockQueueBundle.h"
#include "../include/mimo/Entity.h"
#include "../src/depots/SingleJobDepot.h"


using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

MOCK_FACTORY1(MockQueueBundle, mimo::IQueueBundle, std::shared_ptr<workflow::ConnectionMap>)
MOCK_FACTORY2(MockJob, mimo::IJob, std::shared_ptr<workflow::Step>, std::shared_ptr<mimo::Step>)

TEST(SingleJobDepotTest, test_buffer_interactions) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step_id = workflow->add_step("step", {"input"}, {});
    auto input_id = std::dynamic_pointer_cast<workflow::Input>(step_id->get_inputs()->at("input"));
    auto step = std::make_shared<mimo::MockStep>();
    auto entity = std::make_shared<mimo::Entity>();

    mimo::MockQueue queue1;
    mimo::MockQueue queue2;
    EXPECT_CALL(queue1, push_proxy(&queue2))
        .Times(1);
    auto job = new mimo::MockJob();
    auto mock_bundle = new mimo::MockQueueBundle();
    EXPECT_CALL(*mock_bundle, push("input", entity))
        .Times(1);
    EXPECT_CALL(*mock_bundle, get_push_status("input"))
        .WillOnce(Return(mimo::IOutputs::PushStatus::CAN_PUSH))
        .WillOnce(Return(mimo::IOutputs::PushStatus::QUEUE_FULL));
    EXPECT_CALL(*mock_bundle, get_queue("input"))
        .WillOnce(ReturnRef(queue1));
    auto job_factory = std::make_shared<MockJobFactory>();
    EXPECT_CALL(*job_factory, make_raw(_, _)) // TODO: Match against step_id and step.
        .WillOnce(Return(job));
    auto mock_bundle_factory = std::make_shared<MockQueueBundleFactory>();
    EXPECT_CALL(*mock_bundle_factory, make_raw(step_id->get_inputs()))
        .WillOnce(Return(mock_bundle));

    mimo::SingleJobDepot depot(step_id, step, job_factory, mock_bundle_factory);
    depot.push(*input_id, entity);
    EXPECT_TRUE(depot.can_queue(*input_id));
    EXPECT_FALSE(depot.can_queue(*input_id));
    depot.queue_input(*input_id, queue2);
}

TEST(SingleJobDepotTest, test_with_runnable_job) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step_id = workflow->add_step("step", {"input"}, {});
    auto input_id = std::dynamic_pointer_cast<workflow::Input>(step_id->get_inputs()->at("input"));
    auto step = std::make_shared<mimo::MockStep>();
    auto entity = std::make_shared<mimo::Entity>();

    auto mock_bundle = new mimo::MockQueueBundle();
    auto mock_bundle_factory = std::make_shared<MockQueueBundleFactory>();
    EXPECT_CALL(*mock_bundle_factory, make_raw(step_id->get_inputs()))
            .WillOnce(Return(mock_bundle));
    auto mock_job = new mimo::MockJob();
    EXPECT_CALL(*mock_job, can_run())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*mock_job, transfer_input_proxy(mock_bundle))
        .Times(1);
    auto job_factory = std::make_shared<MockJobFactory>();
    EXPECT_CALL(*job_factory, make_raw(_, _))
        .WillOnce(Return(mock_job));

    mimo::SingleJobDepot depot(step_id, step, job_factory, mock_bundle_factory);
    EXPECT_TRUE(depot.has_runnable_jobs());
    auto jobs = depot.get_runnable_jobs();
    EXPECT_FALSE(depot.has_runnable_jobs());
    while (!jobs.empty()) {
        depot.return_exhausted_job(std::move(jobs.back()));
        jobs.pop_back();
    }
    EXPECT_TRUE(depot.has_runnable_jobs());
}

TEST(SingleJobDepotTest, test_with_non_runnable_job) {
    auto workflow = std::make_shared<workflow::Workflow>();
    auto step_id = workflow->add_step("step", {"input"}, {});
    auto input_id = std::dynamic_pointer_cast<workflow::Input>(step_id->get_inputs()->at("input"));
    auto step = std::make_shared<mimo::MockStep>();
    auto entity = std::make_shared<mimo::Entity>();

    auto mock_job = new mimo::MockJob();
    EXPECT_CALL(*mock_job, can_run())
        .WillRepeatedly(Return(false));
    auto job_factory = std::make_shared<MockJobFactory>();
    EXPECT_CALL(*job_factory, make_raw(_, _))
        .WillOnce(Return(mock_job));
    auto mock_bundle = new mimo::MockQueueBundle();
    auto mock_bundle_factory = std::make_shared<MockQueueBundleFactory>();
    EXPECT_CALL(*mock_bundle_factory, make_raw(step_id->get_inputs()))
        .WillOnce(Return(mock_bundle));

    mimo::SingleJobDepot depot(step_id, step, job_factory, mock_bundle_factory);
    EXPECT_FALSE(depot.has_runnable_jobs());
}
