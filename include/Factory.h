/**
 * @author: Liam Childs (liam.h.childs@gmail.com)
 * @brief:
 */

#ifndef MIMO_FACTORY_H
#define MIMO_FACTORY_H

#include "IFactory.h"


namespace mimo {

    template<typename Base, typename Derived, typename ... Args>
    class Factory : public IFactory<Base, Args...> {

        Base* make_raw(Args ... args) const override;

        std::shared_ptr<Base> make_shared(Args ... args) const override;

        std::unique_ptr<Base> make_unique(Args ... args) const override;
    };
}

template<typename Base, typename Derived, typename ... Args>
Base* mimo::Factory<Base, Derived, Args...>::make_raw(Args ... args) const {
    return new Derived(std::forward<Args>(args)...);
}

template<typename Base, typename Derived, typename ... Args>
std::shared_ptr<Base> mimo::Factory<Base, Derived, Args...>::make_shared(Args ... args) const {
    return std::make_shared<Derived>(std::forward<Args>(args)...);
}

template<typename Base, typename Derived, typename ... Args>
std::unique_ptr<Base> mimo::Factory<Base, Derived, Args...>::make_unique(Args ... args) const {
    return std::make_unique<Derived>(std::forward<Args>(args)...);
}


#endif //MIMO_FACTORY_H
