/**
 * An example demonstrating a simple workflow That splits the output of a stream.
 */

#include "Pipeline.h"
#include "entities/Integer.h"
#include "streams/Print.h"
#include "streams/Range.h"


int main() {
    Pipeline pipeline;
    Range range(500);
    Print<Integer> print_left("", " was split left.");
    Print<Integer> print_right("", " was split right.");

    uuid range_id = pipeline.add_stream(range);
    uuid print_left_id = pipeline.add_stream(print_left);
    uuid print_right_id = pipeline.add_stream(print_right);
    pipeline.pipe({range_id, "output"}, {print_left_id, "input"});
    pipeline.pipe({range_id, "output"}, {print_right_id, "input"});
    pipeline.run();

    return 0;
}
