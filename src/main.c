#include <stdio.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

int main(void)
{
    printf("MIRV-Web: It's MIRV or mug!\nMade by Uneven Prankster @ 2020\n");
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr, "[SDL2]: %d\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_Window *win = SDL_CreateWindow("MIRV-Web", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, 
                                             SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP);
    if(!win){
        fprintf(stderr, "[SDL2]: %d\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    if(!ctx){
        fprintf(stderr, "[SDL2]: %d\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return -1;
    }

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)){
        SDL_GL_DeleteContext(ctx);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return -1;
    }

    SDL_GL_SetSwapInterval(0);

    glViewport(0, 0, 1600, 900);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.1, 0.1, 0.1, 1.0);
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

        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}