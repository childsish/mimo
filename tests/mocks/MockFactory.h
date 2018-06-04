/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKFACTORY_H
#define MIMO_MOCKFACTORY_H

#include <gmock/gmock.h>
#include "../../src/IFactory.h"

#define MOCK_FACTORY(Name, Base) \
class Name ## Factory : public mimo::IFactory<Base> { \
public: \
    MOCK_CONST_METHOD0(make_raw, Base *()); \
};

#define MOCK_FACTORY1(Name, Base, Arg1) \
class Name ## Factory : public mimo::IFactory<Base, Arg1> { \
public: \
    MOCK_CONST_METHOD1(make_raw, Base *(Arg1)); \
};

#define MOCK_FACTORY2(Name, Base, Arg1, Arg2) \
class Name ## Factory : public mimo::IFactory<Base, Arg1, Arg2> { \
public: \
    MOCK_CONST_METHOD2(make_raw, Base *(Arg1, Arg2)); \
};

#define MOCK_FACTORY3(Name, Base, Arg1, Arg2, Arg3) \
class Name ## Factory : public mimo::IFactory<Base, Arg1, Arg2, Arg3> { \
public: \
    MOCK_CONST_METHOD3(make_raw, Base *(Arg1, Arg2, Arg3)); \
};

#define MOCK_FACTORY4(Name, Base, Arg1, Arg2, Arg3, Arg4) \
class Name ## Factory : public mimo::IFactory<Base, Arg1, Arg2, Arg3, Arg4> { \
public: \
    MOCK_CONST_METHOD4(make_raw, Base *(Arg1, Arg2, Arg3, Arg4)); \
};

#endif //MIMO_MOCKFACTORY_H
