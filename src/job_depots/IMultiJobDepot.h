/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IMULTIJOBDEPOT_H
#define MIMO_IMULTIJOBDEPOT_H

#include "IJobDepot.h"


namespace mimo {

    class IMultiJobDepot : public IJobDepot {
    public:

        virtual ~IMultiJobDepot() = default;

        virtual void add_entity(
            const std::shared_ptr<workflow::Output> &output,
            std::shared_ptr<Entity> entity
        ) = 0;

    };
}

#endif //MIMO_IMULTIJOBDEPOT_H
