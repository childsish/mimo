#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "StepFactory.h"

#include "Step.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"
#include <workflow/Workflow.h>


class MockStep : public mimo::Step {
public:
    MockStep() {}

    MOCK_METHOD2(run, bool(mimo::Inputs &ins, mimo::Outputs &outs));
};

TEST(StepFactoryTest, test_make) {
    workflow::Workflow workflow;
    mimo::StepFactory factory;

    auto identifier = workflow.add_step("step", {}, {});
    factory.register_step<MockStep>(identifier);
    EXPECT_NO_THROW(factory.make_step<MockStep>(identifier));
}
