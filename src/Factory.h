/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_FACTORY_H
#define MIMO_FACTORY_H

#include "IFactory.h"


namespace mimo {
    template<typename Base, typename Derived, typename ... Args>
    class Factory : public IFactory<Base, Args...> {

        Base* make_raw(Args ... args) const override;
    };
}

template<typename Base, typename Derived, typename ... Args>
Base* mimo::Factory<Base, Derived, Args...>::make_raw(Args ... args) const {
    return new Derived(std::forward<Args>(args)...);
}


#endif //MIMO_FACTORY_H
