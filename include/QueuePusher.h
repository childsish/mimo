#ifndef MIMO_QUEUEPUSHER_H
#define MIMO_QUEUEPUSHER_H

#include <string>
#include <vector>
#include "Entity.h"
#include "Queue.h"


class QueuePusher {
public:

    /**
     * Push entity to all queues and adjust the reference count appropriately if pushing to multiple queues.
     * @param entity entity to push
     * @return true if all queues can still push
     */
    bool push(Entity *entity);

    /**
     * Check all queues if they can push
     * @return true if all queues can push
     */
    bool can_push() const;

    /**
     * Close all queues.
     */
    void close();

    /**
     * Add a queue to be pushed to
     * @param queue queue to add to pusher
     */
    void add_queue(Queue *queue);

private:

    std::vector<Queue *> _queues;

};


#endif //MIMO_QUEUEPUSHER_H
