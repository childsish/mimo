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

        explicit Outputs(bool synchronous);

        void add_queue(const std::string &name, std::unique_ptr<mimo::Queue> queue);

        std::unique_ptr<mimo::Queue> release_queue(const std::string &name);

        bool can_push() const;

        mimo::OutputQueue &operator[](const std::string name);

        std::unordered_map<std::string, mimo::OutputQueue>::iterator begin() const;

        std::unordered_map<std::string, mimo::OutputQueue>::iterator end() const;

    private:

        bool synchronous;

        std::unordered_map<std::string, mimo::OutputQueue> queues;

    };
}


#endif //MIMO_OUTPUTS_H
