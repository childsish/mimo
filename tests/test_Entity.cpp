#include "gtest/gtest.h"

#include "Entity.h"


class Integer : public Entity {
public:
    int value;

    Integer(int value_) : value(value_) {}
};


TEST(EntityTest, test_init) {
    Entity *entity = new Entity();

    ASSERT_EQ(1, entity->reference_count);

    entity->reference_count--; // Prevent warning message
    delete entity;
}

TEST(EntityTest, test_inherit) {
    Integer *integer = new Integer(0);

    ASSERT_EQ(1, integer->reference_count);

    integer->reference_count--; // Prevent warning message
    delete integer;
}
