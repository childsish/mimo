/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_IFACTORY_H
#define MIMO_IFACTORY_H

#include <memory>


namespace mimo {

    template<typename T, typename ... Args>
    class IFactory {
    public:
        virtual ~IFactory() = default;

        virtual T* make_raw(Args ... args) const = 0;

        virtual std::shared_ptr<T> make_shared(Args ... args) const = 0;

        virtual std::unique_ptr<T> make_unique(Args ... args) const = 0;
    };
}

#endif //MIMO_IFACTORY_H
