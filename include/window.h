#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>

typedef struct{
    SDL_Window* win;
    SDL_GLContext ctx;
    uint32_t w, h;
}mrv_window;

mrv_window* mirv_create_win(const uint32_t width, const uint32_t height);
void mirv_destroy_win(mrv_window* window);