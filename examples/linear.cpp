/**
 * An example demonstrating a simple workflow containing a linear combination of steps. Each step
 * has a maximum of one input and one output and the outputs are never split.
 */

#include <workflow/Workflow.h>
#include <mimo/Engine.h>
#include "steps/Print.h"
#include "steps/Range.h"


int main() {
    auto range = std::make_shared<Range>(0, 1000000, 1);
    auto print = std::make_shared<Print<Integer>>();

    auto workflow = std::make_shared<workflow::Workflow>();
    auto range_step = workflow->add_step(range->get_name(), range->get_inputs(), range->get_outputs());
    auto print_step = workflow->add_step(print->get_name(), print->get_inputs(), print->get_outputs());
    range_step->pipe(print_step);

    mimo::Engine engine;
    engine.register_step(range_step, range);
    engine.register_step(print_step, print);
    engine.run(workflow);

    return 0;
}
