/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IJOBDEPOT_H
#define MIMO_IJOBDEPOT_H

#include <memory>
#include <set>
#include <workflow/Input.h>


namespace mimo {

    class Entity;
    class IJob;
    class IQueue;

    /** @brief Storage for jobs that controls their release and access to their input queue. */
    class IJobDepot {
    public:
        virtual ~IJobDepot() = default;

        /** @brief Add a single entity to the specified input queue of the job. */
        virtual void push(
            const std::shared_ptr<workflow::Input> &input_id,
            std::shared_ptr<Entity> entity
        ) = 0;

        /** @brief Check if there are jobs considered runnable in the local context. */
        virtual bool has_runnable_jobs() const = 0;

        /** @brief Get the job if it considered runnable in the local context. */
        virtual std::set<std::unique_ptr<IJob>> get_runnable_jobs() = 0;

        /** @brief Return the job to the depot. */
        virtual void return_exhausted_job(std::unique_ptr<IJob> job) = 0;

    };
}

#endif //MIMO_IJOBDEPOT_H
