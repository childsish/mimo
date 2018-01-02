/**
 * An example demonstrating a simple workflow containing a linear combination of steps. Each step has one input and
 * one output and the outputs are never split.
 */

#include <workflow/Workflow.h>
#include "Engine.h"
#include "steps/Print.h"
#include "steps/Range.h"


int main() {
    auto workflow = std::make_unique<workflow::Workflow>();
    auto range = workflow->add_step("range", {}, {"output"});
    auto print = workflow->add_step("print", {"input"}, {});
    range->pipe(print);

    mimo::Engine engine;
    engine.register_step(range, [](){ return std::make_unique<Range>(); });
    engine.register_step<Print>(print);
    engine.run(workflow);

    return 0;
}
