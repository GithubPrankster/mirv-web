#pragma once
#include "script.h"

int lua_scr_clean(lua_State *L);

#define MRV_LUAFS 1
const mrv_fpair lua_funcs[MRV_LUAFS] = 
{
    {"scr_clean", lua_scr_clean}
};

#define reg_cfunc(L, func, name){   \
    lua_pushcfunction(L, func);     \
    lua_setglobal(L, name);         \
}