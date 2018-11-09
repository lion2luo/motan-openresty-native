//
// Created by minggang on 2018/11/9.
//

#ifndef MOTAN_LUA_MOTAN_H
#define MOTAN_LUA_MOTAN_H

#include "lua.h"

#define MOTAN_OK 0
#define E_MOTAN_BUFFER_NOT_ENOUGH -1
#define E_MOTAN_OVERFLOW -2
#define E_MOTAN_UNSUPPORTED_TYPE -3
#define E_MOTAN_MEMORY_NOT_ENOUGH -4

static inline int motan_version(lua_State *L) {
    lua_pushstring(L, "0.0.1");
    return 1;
}

static const char *motan_error(int err) {
    switch (err) {
        case MOTAN_OK:
            return "ok";
        case E_MOTAN_BUFFER_NOT_ENOUGH:
            return "motan buffer not enough";
        case E_MOTAN_OVERFLOW:
            return "motan number overflow";
        case E_MOTAN_UNSUPPORTED_TYPE:
            return "motan unsupported type";
        case E_MOTAN_MEMORY_NOT_ENOUGH:
            return "motan memory not enough";
        default:
            return "unknown error";
    }
}

#endif //MOTAN_LUA_MOTAN_H
