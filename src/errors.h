/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_ERRORS_H
#define MIMO_ERRORS_H


#include <stdexcept>

namespace mimo {

    class QueueError : std::runtime_error {
    public:

        explicit QueueError(const std::string &message) : std::runtime_error(message) {}
    };

    class JobManagerError : std::runtime_error {
    public:

        explicit JobManagerError(const std::string &message) : std::runtime_error(message) {}
    };
}

#endif //MIMO_ERRORS_H
