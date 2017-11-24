/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_OUTPUTS_H
#define MIMO_OUTPUTS_H

#include <string>
#include "queues/Queue.h"
#include "OutputQueue.h"

namespace mimo {

    class Outputs {
    public:

        bool can_push() const;

        mimo::OutputQueue &operator[](const std::string name);

    private:

        std::unordered_map<std::string, mimo::OutputQueue> queues;

    };
}


#endif //MIMO_OUTPUTS_H
