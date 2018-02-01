/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_IJOBMANAGER_H
#define MIMO_IJOBMANAGER_H

#include <memory>


namespace workflow {
    class Input;
}

namespace mimo {

    class Entity;
    class Job;

    class IJobManager {
    public:

        template<typename T, typename... P>
        void register_step(std::shared_ptr<workflow::Step> identifier, P&&... args);

        virtual void add_entity(const std::shared_ptr<workflow::Input> identifier,
                                std::shared_ptr<Entity> entity) = 0;

        virtual void add_entity(const std::shared_ptr<workflow::Output> identifier,
                                std::shared_ptr<Entity> entity) = 0;

        virtual bool has_job() const = 0;

        virtual std::unique_ptr<Job> get_job() = 0;
    };
}


#endif //MIMO_IJOBMANAGER_H
