/** @author Liam Childs (liam.h.childs@gmail.com) */
#ifndef MIMO_ENTITY_H
#define MIMO_ENTITY_H

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace mimo {

    /**
     * @brief The basic entity class.
     * Attributes are used to track meta-information about the entity.
     */
    class Entity {
    public:
        std::unordered_map<std::string, std::unordered_set<std::string>> attributes;
    };
}

#endif //MIMO_ENTITY_H
