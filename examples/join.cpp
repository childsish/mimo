/**
 * An example demonstrating a simple workflow that joins two streams. The outputs are never split.
 */

#include "steps/Multiply.h"
#include "steps/Print.h"
#include "steps/Range.h"
#include <mimo/Engine.h>


int main() {
    auto range_forward = std::make_shared<Range>(0, 1000, 1);
    auto range_backward = std::make_shared<Range>(1000, 0, -1);
    auto multiply = std::make_shared<Multiply>();
    auto print = std::make_shared<Print<Integer>>();

    auto workflow = std::make_shared<workflow::Workflow>();
    auto range_forward_step = workflow->add_step(
        range_forward->get_name(),
        range_forward->get_inputs(),
        range_forward->get_outputs());
    auto range_backward_step = workflow->add_step(
        range_backward->get_name(),
        range_backward->get_inputs(),
        range_backward->get_outputs());
    auto multiply_step = workflow->add_step(
        multiply->get_name(),
        multiply->get_inputs(),
        multiply->get_outputs());
    multiply_step->synchronise_inputs({"multiplicand", "multiplier"});
    auto print_step = workflow->add_step(
        print->get_name(),
        print->get_inputs(),
        print->get_outputs());
    range_forward_step->pipe(*multiply_step->get_input("multiplicand"));
    range_backward_step->pipe(*multiply_step->get_input("multiplier"));
    multiply_step->pipe(*print_step);

    mimo::Engine engine;
    engine.register_step(range_forward_step, range_forward);
    engine.register_step(range_backward_step, range_backward);
    engine.register_step(multiply_step, multiply);
    engine.register_step(print_step, print);
    engine.run(workflow);

    return 0;
}
