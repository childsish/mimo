/**
 * An example demonstrating a simple workflow that has one stream with multiple outputs. The outputs are never split.
 */

#include "Pipeline.h"
#include "entities/Integer.h"
#include "streams/Print.h"
#include "streams/Range.h"

template<typename E>
class Split : public Stream {
public:

    Split(bool (*condition)(const E *value)) : Stream("split", {"value"}, {"true", "false"}), _condition(condition) {}

    void run() {
        E *value;
        while (can_run()) {
            value = static_cast<E *>(inputs["value"].pop());
            if (_condition(value)) {
                outputs["true"].push(value);
            }
            else {
                outputs["false"].push(value);
            }
        }
    }

private:

    bool (*_condition)(const E *value);

};

bool is_even(const Integer *integer) {
    return integer->value % 2 == 0;
}


int main() {
    Queue::THRESHOLD = 2;

    Pipeline pipeline;
    Range range(10);
    Split<Integer> split(is_even);
    Print<Integer> print_true("", " is even");
    Print<Integer> print_false("", " is not even");

    uuid range_id = pipeline.add_stream(range);
    uuid split_id = pipeline.add_stream(split);
    uuid print_true_id = pipeline.add_stream(print_true);
    uuid print_false_id = pipeline.add_stream(print_false);
    pipeline.pipe({range_id, "output"}, {split_id, "value"});
    pipeline.pipe({split_id, "true"}, {print_true_id, "input"});
    pipeline.pipe({split_id, "false"}, {print_false_id, "input"});
    pipeline.run();

    return 0;
}
