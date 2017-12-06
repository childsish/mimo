#ifndef MIMO_RANGE_H
#define MIMO_RANGE_H

#include "Step.h"
#include "entities/Integer.h"

namespace mimo {

    class Range : public Step {
    public:

        Range(int to) :
                Step("range", {}, {"output"}),
                _from(0),
                _to(to),
                _step(1)
        {
            if (to < 0) {
                throw std::runtime_error("Parameter to must be positive.");
            }
        }
        Range(int from, int to, int step = 1) :
                Step("range", {}, {"output"}),
                _from(from),
                _to(to),
                _step(step) {
            if (step == 0) {
                throw std::runtime_error("Parameter step must be non-zero.");
            }
        }

        void run() {
            while (can_run()) {
                outputs["output"].push(std::static_pointer_cast<Entity>(std::make_shared<Integer>(_from)));
                _from += _step;
                if (_step > 0 && _from >= _to) {
                    outputs["output"].close();
                }
                else if (_step < 0 && _from <= _to) {
                    outputs["output"].close();
                }
            }
        }

    private:

        int _from;
        int _to;
        int _step;

    };
}

#endif //MIMO_RANGE_H
