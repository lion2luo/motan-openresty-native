//
// Created by minggang on 2018/11/9.
//

#ifndef MOTAN_LUA_SERIALIZATION_H
#define MOTAN_LUA_SERIALIZATION_H

#include "lua.h"

typedef enum {
    T_NULL = 0,
    T_STRING = 1,
    T_STRING_MAP = 2,
    T_BYTE_ARRAY = 3,
    T_STRING_ARRAY = 4,
    T_BOOL = 5,
    T_BYTE = 6,
    T_INT16 = 7,
    T_INT32 = 8,
    T_INT64 = 9,
    T_FLOAT32 = 10,
    T_FLOAT64 = 11,

    T_MAP = 20,
    T_ARRAY = 21
} motan_simple_serialization_type_t;

// this function will serialize all input parameters, input must be an array of all parameters, return a string and a error if possible
extern int motan_simple_serialize(lua_State *L);

// this function will deserialize all parameters, return a parameter array and a error if possible
extern int motan_simple_deserialize(lua_State *L);

#endif //MOTAN_LUA_SERIALIZATION_H
