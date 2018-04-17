/**
 * @author Liam Childs (liam.h.childs@gmail.com)
 * The basic entity class. All entities implement reference counting and attributes. Reference counting is
 * automatically taken care of by Queues. Attributes are used to track meta-information about the entity.
 */
#ifndef MIMO_ENTITY_H
#define MIMO_ENTITY_H

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace mimo {

    class Entity {
    public:

        std::unordered_map<std::string, std::unordered_set<std::string>> attributes;

    };
}

#endif //MIMO_ENTITY_H
