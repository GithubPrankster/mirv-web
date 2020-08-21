#include "window.h"

mrv_window* mirv_create_win(const uint32_t width, const uint32_t height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr, "[SDL2]: %d\n", SDL_GetError());
        return NULL;
    }

    mrv_window* wn = malloc(sizeof wn);
    if(!wn){
        fprintf(stderr, "[MONROE]: Could not allocate window data.\n");
        SDL_Quit();
        return NULL;
    }

    wn->w = width;
    wn->h = height;

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    wn->win = SDL_CreateWindow("MIRV-Web", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wn->w, wn->h, 
                                             SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP);
    if(!wn->win){
        fprintf(stderr, "[SDL2]: %d\n", SDL_GetError());
        SDL_Quit();
        free(wn);
        return NULL;
    }

    wn->ctx = SDL_GL_CreateContext(wn->win);
    if(!wn->ctx){
        fprintf(stderr, "[SDL2]: %d\n", SDL_GetError());
        SDL_DestroyWindow(wn->win);
        SDL_Quit();
        free(wn);
        return NULL;
    }

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)){
        mirv_destroy_win(wn);
        return NULL;
    }

    SDL_GL_SetSwapInterval(0);

    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return wn;
}

void mirv_destroy_win(mrv_window* window)
{
    if(window){
        if(window->ctx)
            SDL_GL_DeleteContext(window->ctx);
        if(window->win)
            SDL_DestroyWindow(window->win);
        free(window);
    }
}