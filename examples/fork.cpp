/**
 * An example demonstrating a simple workflow that has one stream with multiple outputs. The outputs are never split.
 */

#include "steps/Print.h"
#include "steps/Range.h"
#include "steps/Fork.h"
#include <mimo/Engine.h>
#include <workflow/Workflow.h>


bool is_even(const Integer &integer) {
    return integer.value % 2 == 0;
}

int main() {
    auto range = std::make_shared<Range>(0, 100000, 1);
    auto fork = std::make_shared<Fork<Integer>>(&is_even);
    auto print_true = std::make_shared<Print<Integer>>("", " is even.");
    auto print_false = std::make_shared<Print<Integer>>("", " is odd.");

    auto workflow = std::make_shared<workflow::Workflow>();
    auto range_step = workflow->add_step(range->get_name(), range->get_inputs(), range->get_outputs());
    auto fork_step = workflow->add_step(fork->get_name(), fork->get_inputs(), fork->get_outputs());;
    auto print_true_step = workflow->add_step(print_true->get_name(), print_true->get_inputs(), print_true->get_outputs());
    auto print_false_step = workflow->add_step(print_false->get_name(), print_false->get_inputs(), print_false->get_outputs());
    range_step->pipe(*fork_step);
    fork_step->get_output("true")->pipe(*print_true_step);
    fork_step->get_output("false")->pipe(*print_false_step);

    mimo::Engine engine;
    engine.register_step(range_step, range);
    engine.register_step(fork_step, fork);
    engine.register_step(print_true_step, print_true);
    engine.register_step(print_false_step, print_false);
    engine.run(workflow);

    return 0;
}
