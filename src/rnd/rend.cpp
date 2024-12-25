#include "rend.hpp"
#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>

#define DEFAULT_W 1600
#define DEFAULT_H 1200

Renderer::Renderer() { // TODO do fov by hand
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

	ReinitPerspective(w, h, 100);
}

Renderer::~Renderer() {
	SDL_GL_DeleteContext(mContext);
	mContext = nullptr;
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
}

void Renderer::DoSceneDraws() {
	gSceneRootNode->ApplyFuncToChildren(
		[](Object* o) -> void {
				if (o->CanBecome<Drawable>()) {
					dynamic_cast<Drawable*>(o)->Draw();
				}
			}
		);
}

void Renderer::ReinitPerspective(int w, int h, float fov) {
	const float DEG2RAD = SDL_acos(-1.0f) / 180;
	const float front = 0.1;
	const float back = 1000;

	float tangent = SDL_tan(fov/2 * DEG2RAD);		// tangent of half fovY
	float top = front * tangent;					// half height of near plane
	float right = top * (float(w)/float(h));	// half width of near plane

	glMatrixMode(GL_PROJECTION);
	glFrustum(-right, right, -top, top, front, back);
}
