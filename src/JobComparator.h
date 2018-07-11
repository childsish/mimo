/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_JOBCOMPARATOR_H
#define MIMO_JOBCOMPARATOR_H

#include "IJob.h"


namespace mimo {
    class JobComparator {
    public:
        bool operator()(std::unique_ptr<IJob> const &left, std::unique_ptr<IJob> const &right) const;
    };
}


#endif //MIMO_JOBCOMPARATOR_H
