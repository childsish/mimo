/**
 * An example demonstrating a simple workflow containing a linear combination of streams. Each stream has one input and
 * one output and the outputs are never split.
 */

#include "Pipeline.h"
#include "entities/Integer.h"
#include "streams/Print.h"
#include "streams/Range.h"


int main() {
    Pipeline pipeline;
    Range range(1000);
    Print<Integer> print;

    uuid range_id = pipeline.add_stream(range);
    uuid print_id = pipeline.add_stream(print);
    pipeline.pipe({range_id, "output"}, {print_id, "input"});
    pipeline.run();

    return 0;
}
