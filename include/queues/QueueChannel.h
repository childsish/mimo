/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_QUEUECHANNEL_H
#define MIMO_QUEUECHANNEL_H


#include <unordered_set>
#include "Queue.h"

namespace mimo {

    class QueueChannel {
    public:

        static unsigned int CAPACITY;

        const unsigned int capacity;

        explicit QueueChannel(unsigned int capacity = CAPACITY);

        bool reserve(std::unique_ptr<mimo::Queue> &queue);

        void push(std::unique_ptr<Queue> queue);

        std::unique_ptr<Queue> pop();

        bool can_pop() const;

    private:

        unsigned int current_index;

        std::unordered_map<unsigned int, std::unique_ptr<mimo::Queue>> queues;

        std::unordered_set<unsigned int> reservations;

        inline unsigned int usage() const;

    };
}


#endif //MIMO_QUEUECHANNEL_H
