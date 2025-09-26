#pragma once
#include "structs.h"
#include <string.h>  // for memcpy

#define NEW_COMPONENT(ptr_array, entity, Type, ...) \
    (ptr_array)[entity] = memcpy(malloc(sizeof(Type)), &(Type){__VA_ARGS__}, sizeof(Type))

Entity createEntity();