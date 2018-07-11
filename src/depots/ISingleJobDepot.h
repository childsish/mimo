/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_ISINGLEJOBDEPOT_H
#define MIMO_ISINGLEJOBDEPOT_H

#include "IJobDepot.h"
#include "../IFactory.h"


namespace mimo {

    class ISingleJobDepot : public IJobDepot {
    public:
        /** @brief Check if specified input queue is full. */
        virtual bool can_queue(const workflow::Input &input_id) = 0;

        /** @brief Transfer all entites from the given queue to the specified input. */
        virtual void queue_input(
            const workflow::Input &input_id,
            const IQueue &queue
        ) = 0;
    };
}

#endif //MIMO_ISINGLEJOBDEPOT_H
