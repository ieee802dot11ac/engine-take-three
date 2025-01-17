#include "mesh.hpp"
#include "SDL2/SDL_opengl.h"
#include <cstring>

uint Mesh::display_list_num = 0;

Mesh::Mesh() : mVerts(0), mFaces(0), mTexture(nullptr), mDisplayListId(std::nullopt) {}

Mesh::~Mesh() {
    mTexture.reset();
    if (mDisplayListId.has_value() && glIsList(mDisplayListId.value())) {
        glDeleteLists(mDisplayListId.value(), 1);
        if (mDisplayListId.value() == display_list_num) display_list_num--;
    }
}

void Mesh::Draw() {
    if (mDisplayListId.has_value()) {
        glCallList(mDisplayListId.value());
    } else {
        if (mTexture.get() != nullptr) {
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(mTexture->texture_id);
        }
        for (const Face& f : mFaces) {
            Vtx& v0 = mVerts[f[0]], v1 = mVerts[f[1]], v2 = mVerts[f[2]];
            glTexCoord2fv(*v0.Uv());
            glNormal3fv(*v0.Norm());
            glVertex3fv(*v0.Pos());
            glTexCoord2fv(*v1.Uv());
            glNormal3fv(*v1.Norm());
            glVertex3fv(*v1.Pos());
            glTexCoord2fv(*v2.Uv());
            glNormal3fv(*v2.Norm());
            glVertex3fv(*v2.Pos());
        }
        glDisable(GL_TEXTURE_2D);
    }
}

void Mesh::InitDisplayList() {
    glNewList(display_list_num++, GL_COMPILE);
    mDisplayListId = display_list_num;

    glInterleavedArrays(GL_T2F_N3F_V3F, 0, mVerts.data());

    if (mTexture.get() != nullptr) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(mTexture->texture_id);
    }

    glDrawElements(GL_TRIANGLES, mFaces.size(), GL_UNSIGNED_SHORT, mFaces.data());
    
    glDisable(GL_TEXTURE_2D);
    glEndList();
}
