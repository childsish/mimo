/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTS_H
#define MIMO_INPUTS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace workflow {
    class Input;
}

namespace mimo {

    class IQueue;

    /**
     * @brief:
     */
    class Inputs {
    public:

        Inputs(
            const std::unordered_map<std::string, std::shared_ptr<workflow::Input>> &inputs,
            std::unordered_map<std::string, std::unique_ptr<IQueue>> &queues
        );

        void synchronise_queues(const std::vector<std::string> &group);

        bool can_pop() const;

        std::unique_ptr<mimo::IQueue> &operator[](const std::string &name);

        bool is_empty() const;

        bool is_closed() const;

    private:

        unsigned int group_id;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<std::string, std::unique_ptr<mimo::IQueue>> queues;

    };
}


#endif //MIMO_INPUTS_H
