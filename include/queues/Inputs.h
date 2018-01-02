/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <string>
#include "InputQueue.h"
#include "queues/Queue.h"

namespace mimo {

    class Inputs {
    public:

        Inputs();

        void add_queue(const std::shared_ptr<workflow::Input> &identifier);

        std::unique_ptr<mimo::Queue> release_queue(const std::string &name);

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
