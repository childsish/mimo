/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "Job.h"

mimo::Job::Job(mimo::Step &step_, mimo::Inputs &ins_, mimo::Outputs &outs_) :
    step(step_),
    ins(ins_),
    outs(outs_) {}

void mimo::Job::run() {
    this->completed = this->step.run(this->ins, this->outs);
}
