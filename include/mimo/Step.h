/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_STEP_H
#define MIMO_STEP_H

#include "IInputs.h"
#include "IOutputs.h"


namespace mimo {

    class Step {
    public:

        virtual const std::string &get_name() const = 0;
        virtual const std::vector<std::string> &get_inputs() const = 0;
        virtual const std::vector<std::string> &get_outputs() const = 0;

        /** @brief Run the step. Returns true if the step completed running. */
        virtual bool run(IInputs &ins, IOutputs &outs) = 0;

    };
}

#endif //MIMO_STEP_H
