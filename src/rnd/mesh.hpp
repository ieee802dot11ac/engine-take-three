#pragma once

#include "bases/draw.hpp"
#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "math/vec.hpp"
#include "math/xfm.hpp"
#include "rnd/tex.hpp"
#include <cmath>
#include <memory>
#include <optional>
#include <queue>

struct Vtx {
	float_t x, y, z;
	float_t nx, ny, nz;
	float_t u, v;
	uint32_t bone_idx;
	float_t bone_weight;

	Vector3 *Pos() { return (Vector3 *)&x; }
	Vector3 *Norm() { return (Vector3 *)&nx; }
	Vector2 *Uv() { return (Vector2 *)&u; }

	struct alignas(32) VtxGlData { // InterleavedArrays you have forced my hand
		float_t u, v;			   // 0x8
		float_t nx, ny, nz;		   // 0x14
		float_t x, y, z;		   // 0x20
	};

	VtxGlData ToGLForm() {
		VtxGlData vert;
		vert.u = u;
		vert.v = v;
		vert.nx = nx;
		vert.ny = ny;
		vert.nz = nz;
		vert.x = x;
		vert.y = y;
		vert.z = z;

		return vert;
	}
};

struct Face {
	uint16_t idx[3];

	operator const uint16_t *() const { return idx; }
	uint16_t operator[](uint i) const {
		assert(i < 3);
		return idx[i];
	}
};

struct Bone {
	typedef uint32_t BoneIndex;

	Transform mTransform;
	BoneIndex mParent;
	BoneIndex mChildren[4]; // reasonable limit
	std::string mName;
};

class Mesh : public Positionable, public Drawable {
  public:
	Mesh();
	virtual ~Mesh();
	CLASS_NAME_CRUFT(Mesh)
	virtual void Draw();
	virtual void OnMsg(Message &);

	void InitDisplayList();

	std::vector<Vtx> mVerts;
	std::vector<Face> mFaces;
	std::shared_ptr<Texture> mTexture;
	std::optional<uint>
		mDisplayListId; // nonvalue designates a dynamic mesh, i.e. animates

	static std::queue<uint> sDisplayListIds;

	static void Init();
	NEW_OBJECT(Mesh)
	REGISTER
};
