#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "window.h"
#include "script_def.h"

static time_t get_reload_time(const char* f)
{
    struct stat stats;
    stat(f, &stats);
    return stats.st_mtime;
}

mrv_script *mirv_create_script(const char* f)
{
    mrv_script *script = malloc(sizeof script);
    if(!script){
        printf("[MIRV]: Could not allocate scripting for %s.\n", f);
        return NULL;
    }

    script->L = luaL_newstate();
    if(!script->L){
        printf("[MIRV]: Could not allocate Lua state for %s.\n", f);
        free(script);
        return NULL;
    }

    size_t path_len = strlen(f);
    script->file = malloc(path_len + 1);

    int i;
    for(i = 0; f[i] != '\0'; ++i)
        script->file[i] = f[i];
    script->file[i] = '\0';

    luaL_openlibs(script->L);

    /* 
        I was gonna use the Lua functions and tables for a module, but it's
        awfully documented, and the only good info is on *dynamic libs*.
        HELL NO.

        Besides, the similar ResidualVM implementation of this kind of thing
        follows a similar concept.
    */
    for(int i = 0; i < MRV_LUAFS; ++i)
        reg_cfunc(script->L, lua_funcs[i].f, lua_funcs[i].name)

    script->timer = get_reload_time(script->file);

    return script;
}

void mirv_run_script(mrv_script *script)
{
    luaL_dofile(script->L, script->file);
}

/* Lifted wholesale from PIL with a few adjustments. Srry. */
void mirv_call_script_fun(mrv_script *script, const char *func, const char *sig, ...){
    int nres;
    va_list vl;

    va_start(vl, sig);
    lua_getglobal(script->L, func);

    int narg = 0;
    while(*sig) {
        switch(*sig++) {
            case 'd':
                lua_pushnumber(script->L, va_arg(vl, double));
                break;
            case 'i':
                lua_pushinteger(script->L, va_arg(vl, int));
                break;
            case 's':
                lua_pushstring(script->L, va_arg(vl, char *));
                break;
            case '>':
                goto endwhile;
            default:
                printf("[LUA]: Invalid option (%c).", *(sig - 1));
        }
        ++narg;
        luaL_checkstack(script->L, 1, "[LUA]: Too many arguments.");
    } 

endwhile:
    nres = strlen(sig);
    if (lua_pcall(script->L, narg, nres, 0) != 0)
        printf("[LUA]: Error running function `%s': %s.", func, lua_tostring(script->L, -1));

    nres = -nres;
    while(*sig){
        switch(*sig++){
            case 'd':
                if (!lua_isnumber(script->L, nres))
                    puts("[LUA]: Wrong result type.");
                *va_arg(vl, double *) = lua_tonumber(script->L, nres);
                break;
            case 'i':
                if (!lua_isnumber(script->L, nres))
                    puts("[LUA]: Wrong result type.");
                *va_arg(vl, int *) = (int)lua_tonumber(script->L, nres);
                break;
            case 's':
                if (!lua_isstring(script->L, nres))
                    puts("[LUA]: Wrong result type.");
                *va_arg(vl, const char **) = lua_tostring(script->L, nres);
                break;
            default:
                printf("[LUA]: Invalid option (%c).", *(sig - 1));
        }
        ++nres;
    }
    va_end(vl);
}

void mirv_check_script(mrv_script *script)
{
    time_t t = get_reload_time(script->file);

    if(difftime(t, script->timer) != 0){
        printf("[MIRV]: Script %s has been updated.\n", script->file);
        script->timer = t;
        mirv_run_script(script);
    }
}

void mirv_destroy_script(mrv_script *script)
{
    if(script){
        if(script->L)
            lua_close(script->L);
        if(script->file)
            free(script->file);
        free(script);
    }
}