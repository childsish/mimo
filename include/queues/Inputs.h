/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <memory>
#include <string>
#include <unordered_map>
#include "queues/InputQueue.h"

namespace workflow {
    class Input;
}

namespace mimo {

    class Queue;

    class Inputs {
    public:

        explicit Inputs(const std::unordered_map<std::string, std::shared_ptr<workflow::Input>> &inputs);

        void synchronise_queues(const std::vector<std::string> &group);

        bool can_pop() const;

        mimo::InputQueue &operator[](const std::string &name);

        std::unordered_map<std::string, mimo::InputQueue>::const_iterator begin() const;

        std::unordered_map<std::string, mimo::InputQueue>::const_iterator end() const;

        bool is_empty() const;

        bool is_closed() const;

    private:

        unsigned int group_id;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<std::string, mimo::InputQueue> queues;

    };
}


#endif //MIMO_INPUTS_H
