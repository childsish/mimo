/** @author: Liam Childs (liam.h.childs@gmail.com) */

#include "JobComparator.h"

bool mimo::JobComparator::operator()(
    std::unique_ptr<IJob> const &left,
    std::unique_ptr<IJob> const &right
) const {
    return left->get_job_id() < right->get_job_id();
}
