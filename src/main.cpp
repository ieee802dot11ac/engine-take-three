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

int main() {
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "engine initializing, commit " GIT_COMMIT_HASH);
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JXL);
	atexit(IMG_Quit);

	Renderer r;

	CallClassIniters();
	
	Mesh m;
	m.Reparent(gSceneRootNode);

	Texture* t = new Texture;
	m.mTexture.reset(t);

	{
		uint32_t test_tex[16] = {
			0x000000FF, 0xFFFFFFFF, 0x000000FF, 0xFFFFFFFF,
			0xFFFFFFFF, 0x000000FF, 0xFFFFFFFF, 0x000000FF,
			0x000000FF, 0xFFFFFFFF, 0x000000FF, 0xFFFFFFFF,
			0xFFFFFFFF, 0x000000FF, 0xFFFFFFFF, 0x000000FF,
		};
		t->Activate();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, test_tex);
	}

	m.mVerts.reserve(4);
	m.mVerts[0] = Vtx(0,0,0,0,0,0,0,0);
	m.mVerts[1] = Vtx(1,0,0,0,0,2,0,0);
	m.mVerts[2] = Vtx(0,1,0,0,0,0,2,0);
	m.mVerts[3] = Vtx(1,1,0,0,0,2,2,0);
	m.mFaces.resize(2);
	m.mFaces[0] = Face({0,1,2});
	m.mFaces[1] = Face({1,3,2});
	m.mTransform.mPos = Vector3(0, 0, -2);
	m.mTransform.mRot = Vector3(0, 0, 0);
	m.InitDisplayList();

	while (1) {
		SDL_Event e; // temp until i setup proper event handling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) goto exit_now;
		}
		SDL_FlushEvents(1, -1);
		m.mTransform.mRot.x += 3;
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
