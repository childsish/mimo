#ifndef MIMO_INTEGER_H
#define MIMO_INTEGER_H

#include <ostream>
#include "Entity.h"


class Integer : public mimo::Entity {
public:

    int value;

    explicit Integer(int value_);

};

std::ostream &operator<<(std::ostream &out, const Integer &integer);

#endif //MIMO_INTEGER_H
