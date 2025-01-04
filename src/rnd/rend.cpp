#include "rend.hpp"
#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include "rnd/utl.hpp"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <numbers>

#define DEFAULT_W 1600
#define DEFAULT_H 1200

Renderer::Renderer() { 
	mWindow = SDL_CreateWindow(
		"Engine", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		DEFAULT_W, 
		DEFAULT_H, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);
	mContext = SDL_GL_CreateContext(mWindow);
	SDL_GL_SetSwapInterval(1);

	glViewport(0, 0, DEFAULT_W, DEFAULT_H);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	EnableDepth();

	ReinitPerspective(DEFAULT_W, DEFAULT_H, 100);
}

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
	glClearColor(0.0, 0.0, 0.0, 1.0);

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
	gSceneRootNode->ApplyFuncToChildren(
		[](Object* o) -> void {
				o->DoXIfIs<Drawable>([](Drawable* d) -> void { d->Draw(); });
			}
		);
	glMatrixMode(GL_MODELVIEW);
	glRotatef(2.25, 0,1,1);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1,0,1);
	glVertex3f(0,1,-1.5);
	glColor3f(1,1,0);
	glVertex3f(1,1,-1);
	glColor3f(0,1,1);
	glVertex3f(0,0,-1.5);
	glColor3f(1,0,1);
	glVertex3f(1,0,-1);
	glEnd();
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
}
