/**
 * An example demonstrating a simple workflow that joins two streams. The outputs are never split.
 */

#include "Pipeline.h"
#include "entities/Integer.h"
#include "streams/Print.h"
#include "streams/Range.h"


class Multiply : public Stream {
public:
    Multiply() : Stream("multiply", {"multiplicand", "multiplier"}, {"result"}) {}

    void run() {
        Integer *multiplicand;
        Integer *multiplier;
        while (can_run()) {
            multiplicand = static_cast<Integer *>(inputs["multiplicand"].pop());
            multiplier = static_cast<Integer *>(inputs["multiplier"].pop());
            outputs["result"].push(new Integer(multiplicand->value * multiplier->value));
            collect(multiplicand);
            collect(multiplier);
        }
    }
};

int main() {
    Queue::THRESHOLD = 2;

    Pipeline pipeline;
    Range one_to_n(10);
    Range n_to_one(10, 0, -1);
    Multiply multiply;
    Print<Integer> print;

    uuid one_to_thousand_id = pipeline.add_stream(one_to_n);
    uuid thousand_to_one_id = pipeline.add_stream(n_to_one);
    uuid multiply_id = pipeline.add_stream(multiply);
    uuid print_id = pipeline.add_stream(print);
    pipeline.pipe({one_to_thousand_id, "output"}, {multiply_id, "multiplicand"});
    pipeline.pipe({thousand_to_one_id, "output"}, {multiply_id, "multiplier"});
    pipeline.pipe({multiply_id, "result"}, {print_id, "input"});
    pipeline.run();

    return 0;
}
