/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_IFACTORY_H
#define MIMO_IFACTORY_H

#include <memory>


namespace mimo {

    template<typename T, typename ... Args>
    class IFactory {
    public:
        virtual ~IFactory() = default;

        virtual T* make_raw(Args ... args) const = 0;

        std::shared_ptr<T> make_shared(Args ... args) const;

        std::unique_ptr<T> make_unique(Args ... args) const;
    };
}

template<typename T, typename ... Args>
std::shared_ptr<T> mimo::IFactory<T, Args...>::make_shared(Args ... args) const {
    return std::shared_ptr<T>(this->make_raw(std::forward<Args>(args)...));
}

template<typename T, typename ... Args>
std::unique_ptr<T> mimo::IFactory<T, Args...>::make_unique(Args ... args) const {
    return std::unique_ptr<T>(this->make_raw(std::forward<Args>(args)...));
}

#endif //MIMO_IFACTORY_H
