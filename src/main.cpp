#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>

#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "math/vec.hpp"

int main() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initalizing, commit " GIT_COMMIT_HASH);
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);
    int i = 0;
    while (i++ < 10) {
        (new Object)->Reparent(gSceneRootNode);
    }
    Positionable* p = new Positionable; p->mName = "hai :3";
    p->mPos = Vector3(7,1,5);
    p->mRot = Vector3(6,5,0.2);
    p->mScale = Vector3(6,85,5);
    p->Reparent(gSceneRootNode);
    gSceneRootNode->mChildObjs[6]->Reparent(gSceneRootNode->mChildObjs[4]);
    Positionable* p2 = new Positionable;
    p2->mPos = Vector3(1,6,8);
    p2->mRot = Vector3(5,9,2);
    p2->mScale = Vector3(1,0,0);
    p2->Reparent(gSceneRootNode->FindByName("hai :3"));
    Vector3 pos = p2->WorldPos();
    Vector3 rot = p2->WorldRot();
    Vector3 scl = p2->WorldScl();
    std::cout << "pos test: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
    std::cout << "rot test: " << rot.x << " " << rot.y << " " << rot.z << std::endl;
    std::cout << "scale test: " << scl.x << " " << scl.y << " " << scl.z << std::endl;
    gSceneRootNode->Print(std::cout);
    return 0;
}
