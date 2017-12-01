/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_QUEUECHANNEL_H
#define MIMO_QUEUECHANNEL_H


#include <unordered_set>
#include <bits/unordered_set.h>
#include "Queue.h"

namespace mimo {

    /**
     * A container class for co-ordinating Queues. To ensure that out-of-order Queues are retrieved in-order, a
     * QueueChannel can be used.
     */
    class QueueChannel {
    public:

        static unsigned int CAPACITY;

        const unsigned int capacity;

        /**
         * @param capacity maximum number of queues that can be stored
         */
        explicit QueueChannel(unsigned int capacity = CAPACITY);

        /**
         * Reserve space for a Queue of the given run
         * @param run which run produced the Queue
         */
        void reserve(unsigned int run);

        /**
         * Push a queue to the QueueChannel
         * @param queue
         */
        void push(std::unique_ptr<Queue> queue);

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
         * A reservation can only be made if there is sufficient space or there is only one space left and the run being
         * reserved is the next run.
         * @param run run to reserve space for
         * @return if space can be reserved
         */
        bool can_reserve(unsigned int run) const;

        /**
         * A queue can only be pushed if there is sufficient space or there is only one space left and the run being
         * pushed is the next run. If you can't push, it is also a good idea to check if the run has a reservation.
         * @param run run to push
         * @return if queue can be pushed
         */
        bool can_push(unsigned int run) const;

        /**
         * A queue can only be popped from the channel if it has a run number one more than the previous pop.
         * @return
         */
        bool can_pop() const;

        /**
         * Check if the run has a reservation
         * @param run
         * @return
         */
        bool has_reservation(unsigned int run) const;

    private:

        unsigned int current_reserve;

        unsigned int current_push;

        unsigned int current_pop;

        std::unordered_map<unsigned int, std::queue<std::unique_ptr<mimo::Queue>>> queues;

        std::unordered_set<unsigned int> closed_queues;

        std::unordered_set<unsigned int> reservations;

        inline unsigned int usage() const;

    };

    unsigned int QueueChannel::CAPACITY = 10;

}


#endif //MIMO_QUEUECHANNEL_H
