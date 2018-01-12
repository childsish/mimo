/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBINPUT_H
#define MIMO_JOBINPUT_H

#include <memory>
#include <unordered_map>
#include <vector>


namespace mimo {

    class Entity;
    class IQueue;

    /**
     * @brief: A set of input queues to a step.
     */
    class JobInputs {
    public:

        enum class PopStatus {
            CAN_POP,
            QUEUE_EMPTY,
            SYNC_QUEUE_EMPTY
        };

        JobInputs();

        /**
         * @brief Add a queue to the inputs.
         * @param name Name of the queue.
         * @param queue The queue.
         */
        void add_queue(const std::string &name, std::unique_ptr<IQueue> queue);

        /**
         * @brief Synchronise the named queues.
         */
        void synchronise_queues(const std::vector<std::string> &queues);

        /**
         * Get whether all queues can be popped.
         * @return
         */
        PopStatus get_status() const;

        /**
         * @brief Get whether a queue can be popped or if it, or a synchronised queue, is empty.
         * @param name Queue to query.
         */
        PopStatus get_status(const std::string &name) const;

        /**
         * @brief Peek at the first item in the named queue but do not pop it.
         */
        std::shared_ptr<Entity> peek(const std::string &name);

        /**
         * @brief Pop and return the first item of the named queue.
         */
        std::shared_ptr<Entity> pop(const std::string &name);

        /**
         * @brief Get whether there are any queues in the inputs.
         */
        bool is_empty() const;

        /**
         * @brief Get whether all queues are closed.
         */
        bool is_closed() const;

    private:

        unsigned int group_id;

        std::unordered_map<std::string, std::unique_ptr<IQueue>> queues;

        std::unordered_map<std::string, unsigned int> sync_groups;

        std::unordered_map<unsigned int, bool> get_group_status() const;
    };
}


#endif //MIMO_JOBINPUT_H
