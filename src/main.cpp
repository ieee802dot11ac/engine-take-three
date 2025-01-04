#include <cstdlib>

#include <SDL2/SDL.h>

#include "rnd/rend.hpp"

int main() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initalizing, commit " GIT_COMMIT_HASH);
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);
    Renderer r;
    while (1) {
        SDL_Event e; // temp until i setup proper event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) goto exit_now;
        }
        SDL_FlushEvents(1, -1);
        r.DoSceneDraws();
    }
exit_now:
    return 0;
}
