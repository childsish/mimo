/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#include "Job.h"

mimo::Job::Job(const workflow::Step &identifier_) : identifier(identifier_) {}

void mimo::Job::add_input(const workflow::Input &identifier) {
    _ins.add_queue(identifier);
}

mimo::Inputs &mimo::Job::ins() const {
    return _ins;
}

void mimo::Job::add_output(const workflow::Output &identifier) {
    _outs.add_queue(identifier);
}

mimo::Outputs &mimo::Job::outs() const {
    return _outs;
}

void mimo::Job::run() {
    this->completed = this->step.run(this->_ins, this->_outs);
}

bool mimo::Job::is_complete() const {
    return this->completed;
}
