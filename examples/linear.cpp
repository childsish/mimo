/**
 * An example demonstrating a simple workflow containing a linear combination of streams. Each stream has one input and
 * one output and the outputs are never split.
 */

#include "Pipeline.h"
#include "Entity.h"
#include "Stream.h"
#include "streams/Print.h"


class Integer : public Entity {
public:

    int value;

    Integer(int value_) : value(value_) {}

};

std::ostream &operator<<(std::ostream &out, const Integer &integer) {
    out << integer.value;
    return out;
}


class Range : public Stream {
public:

    Range(int from, int to, int step) :
        Stream("range", {}, {"output"}),
        _from(from),
        _to(to),
        _step(step) {}

    void run() {
        while (can_run()) {
            outputs["output"].push(new Integer(_from));
            _from += _step;
            if (_from >= _to) {
                outputs["output"].close();
            }
        }
    }

private:

    int _from;
    int _to;
    int _step;

};


int main() {
    Pipeline pipeline;
    Range range(0, 1000, 1);
    Print<Integer> print;

    uuid range_id = pipeline.add_stream(range);
    uuid print_id = pipeline.add_stream(print);
    pipeline.pipe({range_id, "output"}, {print_id, "input"});
    pipeline.run();

    return 0;
}
