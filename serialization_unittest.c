//
// Created by minggang on 2018/11/10.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __APPLE__
#define DL_SUFFIX "dylib"
#else
#define DL_SUFFIX "so"
#endif

int main() {
    static char cpath[1024];
    getcwd(cpath, sizeof(cpath));
    strcat(cpath, "/libmotan." DL_SUFFIX);
    setenv("LUA_CPATH", cpath, 1);
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    int stat = luaL_dofile(L, "/data1/test.lua");
    if (stat) {
        perror("do file");
    }
    lua_close(L);
    return 0;
}
