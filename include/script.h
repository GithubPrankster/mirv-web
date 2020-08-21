#pragma once

#include <lauxlib.h>
#include <lualib.h>
#include <luaconf.h>
#include <lua.h>

#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>

typedef struct{
    lua_State *L;
    char *file;
    time_t timer;
}mrv_script;

typedef struct{
    char *name;
    lua_CFunction f;
}mrv_fpair;

mrv_script *mirv_create_script(const char* f);
void mirv_run_script(mrv_script *script);
void mirv_call_script_fun(mrv_script *script, const char *func, const char *sig, ...);
void mirv_check_script(mrv_script *script);
void mirv_destroy_script(mrv_script *script);