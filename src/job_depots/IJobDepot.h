/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBDEPOT_H
#define MIMO_IJOBDEPOT_H

#include <memory>
#include <workflow/Input.h>


namespace mimo {

    class Entity;
    class IJob;

    class IJobDepot {
    public:

        virtual ~IJobDepot() = default;

        virtual void add_entity(
            const std::shared_ptr<workflow::Input> &input,
            std::shared_ptr<Entity> entity
        ) = 0;

        virtual bool has_runnable_job() const = 0;

        virtual std::shared_ptr<IJob> get_runnable_job() = 0;

        virtual void return_complete_job(std::shared_ptr<IJob> job) = 0;

    };
}

#endif //MIMO_IJOBDEPOT_H
