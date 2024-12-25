#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>

#include "bases/obj.hpp"

int main() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initalizing, commit " GIT_COMMIT_HASH);
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);
    int i = 0;
    while (i++ < 10) {
        (new Object)->Reparent(gSceneRootNode);
    }
    gSceneRootNode->mChildObjs[6]->Reparent(gSceneRootNode->mChildObjs[4]);
    gSceneRootNode->Print(std::cout);
    return 0;
}
