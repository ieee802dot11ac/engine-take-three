#pragma once

#include <SDL2/SDL.h>
#include <string>

class Renderer {
  public:
	Renderer(); // also inits perspective mapping n shit
	Renderer(int width, int height, std::string name);
	~Renderer();
	Renderer(const Renderer &&) = delete;
	void DoSceneDraws();

	SDL_Window *mWindow;
	SDL_GLContext mContext;

	static void ReinitPerspective(int w, int h, float fov);
	static bool gDrawAxisHelpers, gWireframe;
};
