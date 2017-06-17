#ifndef MIMO_INTEGER_H
#define MIMO_INTEGER_H

#include <ostream>
#include "Entity.h"


class Integer : public Entity {
public:

    int value;

    Integer(int value_);

};

std::ostream &operator<<(std::ostream &out, const Integer &integer);

#endif //MIMO_INTEGER_H
