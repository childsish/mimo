/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <string>
#include "InputQueue.h"

namespace mimo {

    class Inputs {
    public:

        Inputs();
        Inputs(unsigned int threshold, std::vector<std::string> names);

        bool can_pop() const;

        mimo::InputQueue &operator[](const std::string &name);

    private:

        std::unordered_map<std::string, mimo::InputQueue> queues;

    };
}


#endif //MIMO_INPUTS_H
