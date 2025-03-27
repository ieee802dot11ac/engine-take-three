#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_surface.h>
#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include "os/args.hpp"

#include "rnd/mesh.hpp"
#include "rnd/rend.hpp"
#include "rnd/tex.hpp"

static void CallClassIniters() {
	Mesh::Init();
	Texture::Init();
}

void do_tests(void);

std::string gAppName;

int main(int argc, char **argv) {
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
				"engine initializing, commit " GIT_COMMIT_HASH);
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JXL);
	atexit(IMG_Quit);
	Arguments::Init(argc, const_cast<const char **>(argv));
	if (gAllArgs->ArgBool("test")) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "DOING TESTS");
		CallClassIniters();
		do_tests();
		return 0;
	}

	Renderer r;

	CallClassIniters();

	while (1) {
		SDL_Event e; // temp until i setup proper event handling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				goto exit_now;
		}
		SDL_FlushEvents(1, -1);

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
