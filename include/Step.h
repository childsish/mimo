#ifndef MIMO_STEP_H
#define MIMO_STEP_H

#include <string>
#include <unordered_map>
#include <queues/InputQueue.h>
#include <queues/OutputQueue.h>


class Step {
public:

    /**
     * Run the step. Returns true if the step completed running.
     * @param ins input entities
     * @param outs output entities
     * @return step completed running
     */
    virtual bool run(std::unordered_map<std::string, mimo::InputQueue> ins,
                     std::unordered_map<std::string, mimo::OutputQueue> outs) = 0;

};


#endif //MIMO_STEP_H
