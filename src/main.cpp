#include <cstdlib>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "bases/obj.hpp"
#include "math/vec.hpp"
#include "rnd/mesh.hpp"
#include "rnd/rend.hpp"
#include "rnd/tex.hpp"

static void CallClassIniters() {
    Mesh::Init();
    Texture::Init();
}

int main() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initializing, commit " GIT_COMMIT_HASH);
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);

    Renderer r;

    CallClassIniters();
    
    Mesh m;
    m.Reparent(gSceneRootNode);

    Texture* t = new Texture;
    m.mTexture.reset(t);

    {
        uint8_t test_tex[16] = {
            0x00, 0xFF, 0x00, 0xFF,
            0xFF, 0x00, 0xFF, 0x00,
            0x00, 0xFF, 0x00, 0xFF,
            0xFF, 0x00, 0xFF, 0x00,
        };
        t->Activate();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 4, 4, 0, GL_RED, GL_UNSIGNED_BYTE, test_tex);
    }

    m.mVerts.reserve(3);
    m.mVerts[0] = Vtx(0,0,0,0,0,0,0,0);
    m.mVerts[1] = Vtx(0,0,0,0,0,1,0,0);
    m.mVerts[2] = Vtx(0,0,0,0,0,0,1,0);
    m.mFaces.resize(1);
    m.mFaces[0] = Face({0,1,2});
    m.mTransform.mPos = Vector3(0, 0, -2);
    m.mTransform.mRot = Vector3(0, 0, 0);
    m.InitDisplayList();

    while (1) {
        SDL_Event e; // temp until i setup proper event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) goto exit_now;
        }
        SDL_FlushEvents(1, -1);
        m.mTransform.mRot.z += 3;
        r.DoSceneDraws();
    }
exit_now:
    return 0;
}
