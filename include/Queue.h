/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * A queue for storing entities between streams. Pushing and popping automatically takes care of entity reference
 *  counts.
 */
#ifndef MIMO_QUEUE_H
#define MIMO_QUEUE_H

#include <list>
#include "Entity.h"


class Queue {
public:

    static unsigned int THRESHOLD; // = 100;

    /**
     * Create a queue with a given threshold.
     * Copy construction is expressly deleted.
     * @param threshold maximum
     */
    Queue(unsigned int threshold = THRESHOLD);
    Queue(const Queue &) = delete;
    Queue(const Queue &&other);
    ~Queue();

    std::size_t size() const;

    /**
     * Push an entity into the queue. Return true if there is still space for more.
     * @param entity entity to push
     * @return true if queue size is less than threshold
     */
    bool push(Entity *entity);

    /**
     * Get the next entity, but don't remove it from the queue.
     * @return next entity
     */
    Entity *peek();

    /**
     * Get the next entity and remove it from the queue.
     * @return next entity
     */
    Entity *pop();

    /**
     * Close the queue.
     * Queue can no longer be pushed to and can only be popped until it's empty.
     */
    void close();

    /**
     * Check if queue can be pushed to
     * @return true if fewer entities in queue than threshold
     */
    bool can_push() const;

    /**
     * Check if queue can be popped from
     * @return true if entities in queue
     */
    bool can_pop() const;

private:

    unsigned int _threshold;

    std::list<Entity *> _entities;

    bool _closed;

};


#endif //MIMO_QUEUE_H
