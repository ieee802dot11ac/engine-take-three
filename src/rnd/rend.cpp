#include "rend.hpp"
#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "rnd/utl.hpp"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <numbers>

#define DEFAULT_W 1280
#define DEFAULT_H 960

bool Renderer::gDrawAxisHelpers = true, Renderer::gWireframe = false;

Renderer::Renderer() : Renderer(DEFAULT_W, DEFAULT_H, "Engine") { }

Renderer::Renderer(int w, int h, std::string name) {
	mWindow = SDL_CreateWindow(
		name.c_str(), 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		w, 
		h, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);
	mContext = SDL_GL_CreateContext(mWindow);
	SDL_GL_SetSwapInterval(1);

	glViewport(0, 0, w, h);
	glClearColor(0, 0, 0, 1.0);
	glCullFace(GL_BACK);
	glEnableClientState(GL_VERTEX_ARRAY);

	EnableDepth();

	ReinitPerspective(w, h, 100);
}

Renderer::~Renderer() {
	SDL_GL_DeleteContext(mContext);
	mContext = nullptr;
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
}

void Renderer::DoSceneDraws() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	gSceneRootNode->ApplyFuncToChildren([](Object* o) -> void {
		SDL_assert(o != nullptr);
		o->DoXIfIs<Drawable>([](Drawable* d) -> void {
			glPushMatrix();
			d->DoXIfIs<Positionable>([](Positionable* p) -> void {
				Vector3 pos = p->WorldPos(), rot = p->WorldRot(), scl = p->WorldScl();
				glTranslatef(pos.x, pos.y, pos.z);
				glRotatef(rot.x, 1, 0, 0);
				glRotatef(rot.y, 0, 1, 0);
				glRotatef(rot.z, 0, 0, 1);
				glScalef(scl.x, scl.y, scl.z);
				if (gDrawAxisHelpers) {
					DrawAxisHelper();
				}
			});
			d->Draw();
			glPopMatrix();
		});
	});
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::ReinitPerspective(int w, int h, float fov) {
	constexpr float DEG2RAD = std::numbers::pi / 180;
	const float near = 0.1;
	const float far = 1000;

	float tangent = SDL_tan(fov/2 * DEG2RAD);	// tangent of half fovY
	float top = near * tangent;					// half height of near plane
	float right = top * (float(w)/float(h));	// half width of near plane

	glMatrixMode(GL_PROJECTION);
	glFrustum(-right, right, -top, top, near, far);
	glMatrixMode(GL_MODELVIEW);
}
