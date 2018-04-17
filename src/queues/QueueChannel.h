/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_QUEUECHANNEL_H
#define MIMO_QUEUECHANNEL_H


#include <mutex>
#include <unordered_set>
#include <unordered_map>
#include "Queue.h"

namespace mimo {

    class OutputQueue;

    /**
     * A container class for co-ordinating Queues. To ensure that out-of-order Queues are retrieved in-order, a
     * QueueChannel can be used.
     */
    class QueueChannel {
    public:

        enum PushStatus {
            CAN_PUSH, // can push to channel
            PUSH_FULL, // no space left in channel for push
            PUSH_NEXT, // last place must be pushed by next task
            PUSH_ENDED // queue is from task that has ended
        };

        enum PopStatus {
            CAN_POP,
            CAN_NOT_POP
        };

        static unsigned int CAPACITY;

        const unsigned int capacity;

        /**
         * @param capacity maximum number of queues that can be stored
         */
        explicit QueueChannel(unsigned int capacity = CAPACITY);

        /**
         * Push a queue to the QueueChannel
         * Queues from the same task are expected to be pushed in-order. The last queue must be ended with
         * Queue::end_task.
         * @param queue queue being pushed
         */
        void push(OutputQueue &queue);

        /**
         * Peek at the next Queue in the channel. If the next is not yet available, an exception is thrown.
         * QueueChannel::can_pop can be used to check if the next Queue is available.
         * @return next Queue
         */
        const std::unique_ptr<Queue> &peek() const;

        /**
         * Pop and return the next Queue from the channel. If the next is not available, an exception is thrown.
         * QueueChannel::can_pop can be used to check if the next Queue is available.
         * @return next Queue
         */
        std::unique_ptr<Queue> pop();

        /**
         * A queue can only be pushed if there is sufficient space or there is only one space left and the task being
         * pushed is the next task. If you can't push, it is also a good idea to check if the task has a reservation.
         * @param task task to push
         * @return if queue can be pushed
         */
        PushStatus get_push_status(unsigned int task) const;

        /**
         * A queue can only be popped from the channel if it has a task number one more than the previous pop.
         * @return
         */
        PopStatus get_pop_status() const;

        /**
         * Lock the queue channel
         */
        void lock();

        /**
         * Unlock the queue channel
         */
        void unlock();

    private:

        unsigned int current_push;

        unsigned int current_pop;

        std::unordered_map<unsigned int, std::queue<std::unique_ptr<mimo::Queue>>> queues;

        std::unordered_set<unsigned int> ended_queues;

        inline unsigned long usage() const;

        std::mutex mutex;

    };

}


#endif //MIMO_QUEUECHANNEL_H
