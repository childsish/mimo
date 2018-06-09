/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IMULTIJOBDEPOT_H
#define MIMO_IMULTIJOBDEPOT_H

#include <workflow/Output.h>
#include "IJobDepot.h"


namespace mimo {

    class IMultiJobDepot : public IJobDepot {
    public:
        /** @brief Check if specified input queue is full. */
        virtual bool can_queue(const std::shared_ptr<workflow::Output> &output_id) = 0;

        /** @brief Transfer all entites from the given queue to the inputs connected to the specified output. */
        virtual void queue_input(
            const workflow::Output &output_id,
            const IQueue &queue
        ) = 0;
    };
}

#endif //MIMO_IMULTIJOBDEPOT_H
