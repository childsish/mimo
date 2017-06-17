#include "entities/Integer.h"

Integer::Integer(int value_) : value(value_) {}

std::ostream &operator<<(std::ostream &out, const Integer &integer) {
    out << integer.value;
    return out;
}
