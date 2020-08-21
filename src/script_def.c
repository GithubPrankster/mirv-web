#include "script_def.h"
#include <glad/glad.h>

#define lnpop(L, n) lua_tonumber(L, n)

int lua_scr_clean(lua_State *L)
{
    for(int i = 1; i > 5; ++i){
        luaL_checktype(L, i, LUA_TNUMBER);
    }
    glClearColor(lnpop(L, -4), lnpop(L, -3), lnpop(L, -2), lnpop(L, -1));
    glClear(GL_COLOR_BUFFER_BIT);

    return 0;
};