/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOB_H
#define MIMO_JOB_H

#include <workflow/Step.h>
#include <workflow/Input.h>
#include <workflow/Output.h>
#include "Step.h"
#include "queues/Inputs.h"
#include "queues/Outputs.h"


namespace mimo {

    class Job {
    public:

        const workflow::Step &identifier;

        explicit Job(const workflow::Step &identifier);

        void add_input(const workflow::Input &identifier);

        Inputs &ins() const;

        void add_output(const workflow::Output &identifier);

        Outputs &outs() const;

        void run();

        bool is_complete() const;

    private:

        bool completed;

        Step step;

        Inputs _ins;

        Outputs _outs;

    };
}

#endif //MIMO_JOB_H
