/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_FACTORY_H
#define MIMO_FACTORY_H

#include "IFactory.h"


namespace mimo {

    template<typename T, typename ... Args>
    class Factory : public IFactory<T, Args...> {

        T* make_raw(Args ... args) const override;

        std::shared_ptr<T> make_shared(Args ... args) const override;

        std::unique_ptr<T> make_unique(Args ... args) const override;
    };
}

template<typename T, typename ... Args>
T* mimo::Factory<T, Args...>::make_raw(Args ... args) const {
    return new T(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
std::shared_ptr<T> mimo::Factory<T, Args...>::make_shared(Args ... args) const {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
std::unique_ptr<T> mimo::Factory<T, Args...>::make_unique(Args ... args) const {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

#endif //MIMO_FACTORY_H
