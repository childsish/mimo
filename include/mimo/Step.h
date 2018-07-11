/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_STEP_H
#define MIMO_STEP_H

#include "IInputs.h"
#include "IOutputs.h"
#include <vector>


namespace mimo {

    class Step {
    public:
        Step(
            const std::string &name,
            const std::vector<std::string> &inputs,
            const std::vector<std::string> &outputs
        );

        const std::string &get_name() const;
        const std::vector<std::string> &get_inputs() const;
        const std::vector<std::string> &get_outputs() const;

        /** @brief Run the step. Returns true if the step completed running. */
        virtual bool run(IInputs &ins, IOutputs &outs) = 0;

    private:

        std::string name;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;

    };
}

#endif //MIMO_STEP_H
