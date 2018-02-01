/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include <gtest/gtest.h>
#include "JobManager.h"

#include <workflow/Workflow.h>


TEST(JobManagerTest, test_register_step) {
    workflow::Workflow workflow;
    mimo::JobManager manager(workflow, 2);

    auto step1 = workflow.add_step("step1", {}, {});
    auto step2 = workflow.add_step("step2", {}, {});
    auto step3 = workflow.add_step("step3", {}, {});

    manager.register_step(step1);
}
