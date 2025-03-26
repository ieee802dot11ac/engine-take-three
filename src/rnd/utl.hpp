#pragma once

#include "math/xfm.hpp"
#include <SDL2/SDL_opengl.h>

/// Disables depth testing. Good for UI meshes.
inline void DisableDepth() {
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}
/// Enables depth testing. Tailcalled by UI meshes.
inline void EnableDepth() {
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

inline void DrawAxisHelper() {
	glPushMatrix();
	glScalef(1, 1, 1);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.25, 0, 0);
	glColor3ub(0, 255, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.25, 0);
	glColor3ub(0, 0, 255);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.25);
	glEnd();
	glPopMatrix();
}

inline void ApplyXfm(const Transform &xfm) {
	glTranslatef(xfm.mPos.x, xfm.mPos.y, xfm.mPos.z);
	glRotatef(xfm.mRot.x, 1, 0, 0);
	glRotatef(xfm.mRot.y, 0, 1, 0);
	glRotatef(xfm.mRot.z, 0, 0, 1);
	glScalef(xfm.mScale.x, xfm.mScale.y, xfm.mScale.z);
}
