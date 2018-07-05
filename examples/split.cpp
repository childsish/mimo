/**
 * An example demonstrating a simple workflow That splits the output of a stream.
 */

#include <workflow/Workflow.h>
#include <mimo/Engine.h>
#include "steps/Print.h"
#include "steps/Range.h"


int main() {
    auto range = std::make_shared<Range>(0, 100000, 1);
    auto print_left = std::make_shared<Print<Integer>>("", " was split left.");
    auto print_right = std::make_shared<Print<Integer>>("", " was split right.");

    auto workflow = std::make_shared<workflow::Workflow>();
    auto range_step = workflow->add_step(range->get_name(), range->get_inputs(), range->get_outputs());
    auto print_left_step = workflow->add_step(print_left->get_name(), print_left->get_inputs(), print_left->get_outputs());
    auto print_right_step = workflow->add_step(print_right->get_name(), print_right->get_inputs(), print_right->get_outputs());
    range_step->pipe(print_left_step);
    range_step->pipe(print_right_step);

    mimo::Engine engine;
    engine.register_step(range_step, range);
    engine.register_step(print_left_step, print_left);
    engine.register_step(print_right_step, print_right);
    engine.run(workflow);

    return 0;
}
