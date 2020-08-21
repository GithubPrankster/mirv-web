#include <stdio.h>
#include "window.h"
#include "script.h"

int main(void)
{
    printf("MIRV-Web: It's MIRV or mug!\nMade by Uneven Prankster @ 2020\nVersion dev.08.\n");
    mrv_window *wn = mirv_create_win(1280, 720);
    if(!wn){
        printf("[MIRV]: Could not allocate Windowing.\n");
        return -1;
    }

    mrv_script *script = mirv_create_script("scripts/example.lua");
    mirv_run_script(script);
    mirv_call_script_fun(script, "init", "");
    
    int quitter = 0;
    SDL_Event e;
    while(!quitter){
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quitter = 1;
            }
        }

        const Uint8* cur_keyboard = SDL_GetKeyboardState(NULL);
        if(cur_keyboard[SDL_SCANCODE_ESCAPE]){
            quitter = 1;
        }

        mirv_check_script(script);
        mirv_call_script_fun(script, "update", "d", SDL_GetTicks() / 1000.0);

        SDL_GL_SwapWindow(wn->win);
    }

    mirv_destroy_script(script);
    mirv_destroy_win(wn);
    return 0;
}