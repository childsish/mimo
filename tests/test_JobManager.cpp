/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <gtest/gtest.h>
#include "JobManager.h"

#include <workflow/Workflow.h>
#include "Step.h"


class Step1 : public mimo::Step {
public:
    bool run(mimo::Inputs& ins, mimo::Outputs& outs) override {}
};

TEST(JobManagerTest, test_register_step) {
    workflow::Workflow workflow;
    mimo::JobManager manager(workflow, 2);

    auto step1 = workflow.add_step("step1", {}, {});
    auto step2 = workflow.add_step("step2", {}, {});
    auto step3 = workflow.add_step("step3", {}, {});

    manager.register_step<Step1>(step1);
}
