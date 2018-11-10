//
// Created by minggang on 2018/11/10.
//

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "motan.h"

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_requiref(L, MOTAN_MODNAME, luaopen_cmotan, 0);
    int stat = luaL_dofile(L, "/data1/test.lua");
    if (stat) {
        perror("do file");
    }
    lua_close(L);
    return 0;
}
