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

        Outputs();

        void add_queue(const std::string &name, std::unique_ptr<mimo::Queue> queue);

        std::unique_ptr<mimo::Queue> release_queue(const std::string &name);

        void synchronise_queues(const std::vector<std::string> &group);

        bool can_push() const;

        mimo::OutputQueue &operator[](const std::string &name);

        std::unordered_map<std::string, mimo::OutputQueue>::const_iterator begin() const;

        std::unordered_map<std::string, mimo::OutputQueue>::const_iterator end() const;

    private:

        unsigned int group_id;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<std::string, mimo::OutputQueue> queues;

    };
}


#endif //MIMO_OUTPUTS_H
