/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_JOBMANAGER_H
#define MIMO_JOBMANAGER_H

#include <queue>
#include <list>
#include <workflow/Workflow.h>


namespace mimo {

    class Entity;
    class Job;
    class Step;
    typedef std::function<std::unique_ptr<Step>(void)> StepConstructor;

    /**
     * @brief: Manages the jobs being run by the system. Prevents too many jobs from being run.
     */
    class JobManager {
    public:

        JobManager(const workflow::Workflow &workflow_, unsigned int capacity = 10);

        template<typename T, typename... P>
        void register_step(std::shared_ptr<workflow::Step> identifier, P&&... args) {
            this->constructors.emplace(
                    identifier->identifier,
                    [&args...](){ return std::make_unique<T>(std::forward<P>(args)...); }
            );
        };

        void add_entity(const std::shared_ptr<workflow::Input> input,
                        std::shared_ptr<Entity> entity);

        void add_entity(const std::shared_ptr<workflow::Output> identifier,
                        std::shared_ptr<Entity> entity);

        bool has_job() const;

        std::unique_ptr<Job> get_job();

    private:

        const workflow::Workflow &workflow_;

        unsigned int capacity;

        std::unordered_map<unsigned int, StepConstructor> constructors;

        std::list<std::shared_ptr<workflow::Step>> ready_steps;

        std::unordered_map<unsigned int, std::shared_ptr<Step>> counts;

        std::unordered_map<unsigned int, std::queue<std::shared_ptr<Entity>>> inputs;

        bool step_is_ready(const std::shared_ptr<workflow::Step> step) const;
    };
}


#endif //MIMO_JOBMANAGER_H
