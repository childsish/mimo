#ifndef MIMO_INTEGER_H
#define MIMO_INTEGER_H

#include <ostream>
#include <mimo/Entity.h>


class Integer : public mimo::Entity {
public:
    int value;
    explicit Integer(int value_) : value(value_) {}
};

std::ostream &operator<<(std::ostream &out, const Integer &integer) {
    out << integer.value;
    return out;
}

#endif //MIMO_INTEGER_H
