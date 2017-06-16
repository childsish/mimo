#include "Entity.h"

Entity::Entity : reference_count(1) {}

Entity::~Entity() {
    if (reference_count > 0) {
        std::cerr << "Warning: reference count for entity not 0 upon destruction." << std::endl;
    }
}
