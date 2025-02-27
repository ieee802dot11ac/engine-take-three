#include <SDL2/SDL_log.h>
#include <SDL2/SDL_surface.h>
#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

void do_tests(void);

int main(int argc, char** argv) {
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initializing, commit " GIT_COMMIT_HASH);
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JXL);
	atexit(IMG_Quit);
	if (argc != 1 && strcmp(argv[1], "--test") == 0) { 
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "DOING TESTS"); 
		CallClassIniters(); 
		do_tests(); 
		return 0; 
	}

	Renderer r;

	CallClassIniters();
	
	Mesh m;
	m.Reparent(gSceneRootNode);

	Texture* t = new Texture;
	m.mTexture.reset(t);

	SDL_Surface* srf = IMG_Load("bingus.png");
	SDL_Surface* srf_fixed = SDL_ConvertSurfaceFormat(srf, SDL_PIXELFORMAT_RGB565, 0);
	SDL_FreeSurface(srf);
	t->LoadFromSurface(srf_fixed);
	SDL_FreeSurface(srf_fixed);
	

	m.mVerts.reserve(4);
	m.mVerts[0] = Vtx(0,0,0,0,0,-0.5,0,0.5);
	m.mVerts[1] = Vtx(1,0,0,0,0,0.5,0,0.5);
	m.mVerts[2] = Vtx(0,1,0,0,0,0,1,0);
	m.mVerts[3] = Vtx(1,1,0,0,0,0,0,-0.5);
	m.mFaces.resize(3);
	m.mFaces[0] = Face({0,1,2});
	m.mFaces[1] = Face({1,3,2});
	m.mFaces[2] = Face({3,0,2});
	m.mTransform.mPos = Vector3(0, 0, -3);
	m.mTransform.mRot = Vector3(90, 0, 0);
	m.InitDisplayList();

	while (1) {
		SDL_Event e; // temp until i setup proper event handling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) goto exit_now;
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w) Renderer::gWireframe = !Renderer::gWireframe;
		}
		SDL_FlushEvents(1, -1);
		//m.mTransform.mRot.x += 1;
		m.mTransform.mRot.y -= 3;
		r.DoSceneDraws();

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			do {
				fprintf(stderr, "OpenGL error %04x!\n", err);
			} while ((err = glGetError()) != GL_NO_ERROR);
			fprintf(stderr, "GL error, exiting...\n");
			goto exit_now;
		}
	}
exit_now:
	return 0;
}
