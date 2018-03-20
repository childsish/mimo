/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBMANAGERFACTORY_H
#define MIMO_IJOBMANAGERFACTORY_H

#include <memory>
#include <workflow/Step.h>


namespace mimo {

    class IJobManager;

    class IJobManagerFactory {
    public:
        virtual ~IJobManagerFactory() = default;

        virtual std::unique_ptr<IJobManager> make_manager(
            const std::shared_ptr<workflow::Step> &identifier
        ) const = 0;
    };
}

#endif //MIMO_IJOBMANAGERFACTORY_H
