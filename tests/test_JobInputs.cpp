#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <workflow/Input.h>
#include <Entity.h>
#include <errors.h>
#include "mocks/MockQueue.h"
#include "mocks/MockQueueFactory.h"
#include <queues/JobInputs.h>


using ::testing::Return;

TEST(JobInputsTest, test_get_status_two_full_queues) {
    workflow::InputMap inputs({
        {"queue1", std::make_shared<workflow::Input>("queue1")},
        {"queue2", std::make_shared<workflow::Input>("queue2")}
    });
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *queue1 = new mimo::MockQueue();
    auto *queue2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    EXPECT_CALL(*queue1, can_pop())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*queue2, can_pop())
        .WillRepeatedly(Return(true));

    mimo::JobInputs job_inputs(inputs, factory);
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);
    inputs["queue1"]->sync_group = 1;
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("queue2"), mimo::JobInputs::PopStatus::CAN_POP);
}

TEST(JobInputsTest, test_get_status_one_full_one_empty_queue) {
    workflow::InputMap inputs({
        {"queue1", std::make_shared<workflow::Input>("queue1")},
        {"queue2", std::make_shared<workflow::Input>("queue2")}
    });
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *queue1 = new mimo::MockQueue();
    auto *queue2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    bool queue1_return_value = inputs.begin()->first == "queue1";
    EXPECT_CALL(*queue1, can_pop())
        .WillRepeatedly(Return(queue1_return_value));
    EXPECT_CALL(*queue2, can_pop())
        .WillRepeatedly(Return(!queue1_return_value));

    mimo::JobInputs job_inputs(inputs, factory);
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("queue1"), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("queue2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    inputs["queue1"]->sync_group = 1;
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("queue1"), mimo::JobInputs::PopStatus::CAN_POP);
    EXPECT_EQ(job_inputs.get_status("queue2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
}

TEST(JobInputsTest, test_get_status_two_empty_queues) {
    workflow::InputMap inputs({
        {"queue1", std::make_shared<workflow::Input>("queue1")},
        {"queue2", std::make_shared<workflow::Input>("queue2")}
    });
    auto factory = std::make_shared<mimo::MockQueueFactory>();
    auto *queue1 = new mimo::MockQueue();
    auto *queue2 = new mimo::MockQueue();

    EXPECT_CALL(*factory, make_unique_proxy())
        .WillOnce(Return(queue1))
        .WillOnce(Return(queue2));

    EXPECT_CALL(*queue1, can_pop())
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*queue2, can_pop())
        .WillRepeatedly(Return(false));

    mimo::JobInputs job_inputs(inputs, factory);
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("queue1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("queue2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    inputs["queue1"]->sync_group = 1;
    EXPECT_EQ(job_inputs.get_status(), mimo::JobInputs::PopStatus::SYNC_QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("queue1"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
    EXPECT_EQ(job_inputs.get_status("queue2"), mimo::JobInputs::PopStatus::QUEUE_EMPTY);
}
