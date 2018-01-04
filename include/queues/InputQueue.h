/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_INPUTQUEUE_H
#define MIMO_INPUTQUEUE_H

#include <memory>


namespace workflow {
    class Input;
}

namespace mimo {

    class Entity;
    class Queue;

    /**
     * @brief queue adaptor for queue reading functions
     */
    class InputQueue {
    public:

        const std::shared_ptr<workflow::Input> identifier;

        InputQueue(const std::shared_ptr<workflow::Input> &identifier_, std::unique_ptr<Queue> queue);

        std::shared_ptr<mimo::Entity> peek();
        std::shared_ptr<mimo::Entity> pop();
        bool can_pop() const;

        bool is_closed() const;

    private:

        std::unique_ptr<mimo::Queue> queue;

    };
}


#endif //MIMO_INPUTQUEUE_H
