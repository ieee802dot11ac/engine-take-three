#include "mesh.hpp"
#include <SDL2/SDL_opengl.h>
#include <cstring>

std::queue<uint> Mesh::sDisplayListIds;

void Mesh::Init() {
    constexpr uint list_siz = 1024;

    uint list_start_idx = glGenLists(list_siz);

    for (uint i = list_start_idx; i < list_start_idx + list_siz; i++) {
        sDisplayListIds.push(i);
    }
}

Mesh::Mesh() : mVerts(0), mFaces(0), mTexture(nullptr), mDisplayListId(std::nullopt) {}

Mesh::~Mesh() {
    mTexture.reset();
    if (mDisplayListId.has_value() && glIsList(mDisplayListId.value())) {
        sDisplayListIds.push(mDisplayListId.value());
    }
}

void Mesh::Draw() {
    if (mDisplayListId.has_value()) {
        glCallList(mDisplayListId.value());
    } else {
        if (mTexture.get() != nullptr) {
            glEnable(GL_TEXTURE_2D);
            mTexture->Activate();
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
    if (mDisplayListId.has_value()) { sDisplayListIds.push(mDisplayListId.value()); }
    mDisplayListId = sDisplayListIds.front(); sDisplayListIds.pop();

    glNewList(mDisplayListId.value(), GL_COMPILE);

    glInterleavedArrays(GL_T2F_N3F_V3F, 0, mVerts.data());

    if (mTexture.get() != nullptr) {
        glEnable(GL_TEXTURE_2D);
        mTexture->Activate();
    }

    glDrawElements(GL_TRIANGLES, mFaces.size() * 3, GL_UNSIGNED_SHORT, mFaces.data());
    
    glDisable(GL_TEXTURE_2D);
    glEndList();
}
