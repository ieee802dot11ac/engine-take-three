#pragma once

#include <SDL2/SDL_opengl.h>

/// Disables depth testing. Good for UI meshes.
inline void DisableDepth() { glDisable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); }
/// Enables depth testing. Tailcalled by UI meshes.
inline void EnableDepth() { glEnable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); }
