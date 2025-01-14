#include <cstdlib>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "rnd/rend.hpp"

class Test : public Drawable, public Positionable {
    public:
    Test() {}
    virtual ~Test() {}
    CLASS_NAME_CRUFT(Test)
    virtual void Draw() {
        mRot.z += 3;
        glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glColor3f(0,1,0);
		glVertex3f(0.5,0,0);
		glColor3f(0,0,1);
		glVertex3f(0,0.5,0);
		glColor3f(1,1,1);
		glVertex3f(0.5,0.5,0);
		glEnd();
    }

    void A() { mPos = Vector3(0, -2, -3); }
    void B() { mPos = Vector3(1, 1, -2); mRot.y = 90; }
};

int main() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initializing, commit " GIT_COMMIT_HASH);
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(SDL_Quit);
    Test a, b; a.A(); b.B(); Test c; c.mPos.z = -2;
    a.Reparent(gSceneRootNode);
    b.Reparent(gSceneRootNode);
    c.Reparent(gSceneRootNode);
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
