#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Job.h"

#include <workflow/Step.h>
#include "Entity.h"
#include "Step.h"
#include "queues/IQueue.h"
#include "queues/IJobInputs.h"
#include "queues/IJobOutputs.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


using ::testing::_;
using ::testing::Return;

class MockStep : public mimo::Step {
public:
    MOCK_METHOD2(run, bool(mimo::Inputs&, mimo::Outputs&));
};

class MockJobInputs : public mimo::IJobInputs {
public:
    virtual void add_queue(const std::string &name, std::unique_ptr<mimo::IQueue> queue) {
        this->add_queue_proxy(name, queue.get());
    }

    MOCK_METHOD2(add_queue_proxy, void(const std::string&, mimo::IQueue*));
    MOCK_METHOD1(synchronise_queues, void(const std::vector<std::string>&));
    MOCK_CONST_METHOD0(get_status, PopStatus());
    MOCK_CONST_METHOD1(get_status, PopStatus(const std::string&));
    MOCK_METHOD1(peek, std::shared_ptr<mimo::Entity>(const std::string&));
    MOCK_METHOD1(pop, std::shared_ptr<mimo::Entity>(const std::string&));
    MOCK_CONST_METHOD0(is_empty, bool());
    MOCK_CONST_METHOD0(is_closed, bool());
};

class MockJobOutputs : public mimo::IJobOutputs {
public:
    virtual std::unique_ptr<mimo::IQueue> get_queue(const std::string &name) {
        return std::unique_ptr<mimo::IQueue>(this->get_queue_proxy(name));
    }

    MOCK_METHOD1(get_queue_proxy, mimo::IQueue*(const std::string&));
    MOCK_METHOD1(synchronise_queues, void(const std::vector<std::string>&));
    MOCK_CONST_METHOD0(get_status, PushStatus());
    MOCK_CONST_METHOD1(get_status, PushStatus(const std::string&));
    MOCK_METHOD2(push, void(const std::string&, std::shared_ptr<mimo::Entity>));
    MOCK_METHOD0(end_run, void());
    MOCK_METHOD0(close, void());
    MOCK_CONST_METHOD0(is_closed, bool());
};

TEST(JobTest, test_job_completes_and_closes) {
    auto identifier = std::make_shared<workflow::Step>(0, "step");
    auto step = std::make_unique<MockStep>();
    auto inputs = std::make_unique<MockJobInputs>();
    auto outputs = std::make_unique<MockJobOutputs>();

    EXPECT_CALL(*step, run(_, _))
        .WillOnce(Return(true));
    EXPECT_CALL(*inputs, is_closed())
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*outputs, close())
        .Times(1);
    EXPECT_CALL(*outputs, end_run())
        .Times(1);

    mimo::Job job(
        identifier,
        std::move(step),
        std::move(inputs),
        std::move(outputs)
    );
    job.run();
}

TEST(JobTest, test_job_not_complete) {
    auto identifier = std::make_shared<workflow::Step>(0, "step");
    auto step = std::make_unique<MockStep>();
    auto inputs = std::make_unique<MockJobInputs>();
    auto outputs = std::make_unique<MockJobOutputs>();

    EXPECT_CALL(*step, run(_, _))
            .WillOnce(Return(false));
    EXPECT_CALL(*inputs, is_closed())
            .WillRepeatedly(Return(true));
    EXPECT_CALL(*outputs, close())
            .Times(0);
    EXPECT_CALL(*outputs, end_run())
            .Times(1);

    mimo::Job job(
            identifier,
            std::move(step),
            std::move(inputs),
            std::move(outputs)
    );
    job.run();
}

TEST(JobTest, test_job_complete_but_input_open) {
    auto identifier = std::make_shared<workflow::Step>(0, "step");
    auto step = std::make_unique<MockStep>();
    auto inputs = std::make_unique<MockJobInputs>();
    auto outputs = std::make_unique<MockJobOutputs>();

    EXPECT_CALL(*step, run(_, _))
            .WillOnce(Return(true));
    EXPECT_CALL(*inputs, is_closed())
            .WillRepeatedly(Return(false));
    EXPECT_CALL(*outputs, close())
            .Times(0);
    EXPECT_CALL(*outputs, end_run())
            .Times(1);

    mimo::Job job(
            identifier,
            std::move(step),
            std::move(inputs),
            std::move(outputs)
    );
    job.run();
}
