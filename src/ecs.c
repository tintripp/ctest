#include "ecs.h"

Entity createEntity() {
    static Entity next = 0;
    return next++;
}