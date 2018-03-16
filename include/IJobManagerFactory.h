/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBMANAGERFACTORY_H
#define MIMO_IJOBMANAGERFACTORY_H

#include <memory>


namespace mimo {

    class AsynchronousJobManager;
    class SynchronousJobManager;

    class IJobManagerFactory {
    public:

        ~IJobManagerFactory() = default;

        virtual std::unique_ptr<AsynchronousJobManager> make_asynchronous(
            const std::shared_ptr<workflow::Step> &identifier,
            std::shared_ptr<Step> step
        ) const = 0;

        virtual std::unique_ptr<SynchronousJobManager> make_synchronous(
            const std::shared_ptr<workflow::Step> &identifier,
            std::shared_ptr<Step> step
        ) const = 0;
    };
}

#endif //MIMO_IJOBMANAGERFACTORY_H
