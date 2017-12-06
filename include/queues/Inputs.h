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

        explicit Inputs(bool synchronous);

        void add_queue(const std::string &name, std::unique_ptr<mimo::Queue> queue);

        std::unique_ptr<mimo::Queue> release_queue(const std::string &name);

        bool can_pop() const;

        mimo::InputQueue &operator[](const std::string &name);

        std::unordered_map<std::string, mimo::InputQueue>::iterator begin() const;

        std::unordered_map<std::string, mimo::InputQueue>::iterator end() const;

    private:

        bool synchronous;

        std::unordered_map<std::string, mimo::InputQueue> queues;

    };
}


#endif //MIMO_INPUTS_H
