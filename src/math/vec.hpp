#pragma once

#include <SDL2/SDL_assert.h>
#include <cmath>

struct Vector2 {
	float x, y;

	float Dot(const Vector2 &other) const { return x * other.y + y * other.x; }
	float Length() const { return std::sqrt(x * x + y * y); }

	// could simd these (no.)
	Vector2 &operator+=(const Vector2 &rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2 &operator-=(const Vector2 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vector2 &operator*=(const Vector2 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}
	Vector2 &operator/=(const Vector2 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}
	Vector2 &operator+=(float rhs) {
		x += rhs;
		y += rhs;
		return *this;
	}
	Vector2 &operator-=(float rhs) {
		x -= rhs;
		y -= rhs;
		return *this;
	}
	Vector2 &operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vector2 &operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}

	Vector2 operator+(const Vector2 &rhs) const {
		Vector2 v(x + rhs.x, y + rhs.y);
		return v;
	}
	Vector2 operator-(const Vector2 &rhs) const {
		Vector2 v(x - rhs.x, y - rhs.y);
		return v;
	}
	Vector2 operator*(const Vector2 &rhs) const {
		Vector2 v(x * rhs.x, y * rhs.y);
		return v;
	}
	Vector2 operator/(const Vector2 &rhs) const {
		Vector2 v(x / rhs.x, y / rhs.y);
		return v;
	}
	Vector2 operator+(float rhs) const {
		Vector2 v(x + rhs, y + rhs);
		return v;
	}
	Vector2 operator-(float rhs) const {
		Vector2 v(x - rhs, y - rhs);
		return v;
	}
	Vector2 operator*(float rhs) const {
		Vector2 v(x * rhs, y * rhs);
		return v;
	}
	Vector2 operator/(float rhs) const {
		Vector2 v(x / rhs, y / rhs);
		return v;
	}

	bool operator==(const Vector2 &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vector2 &rhs) const { return !(*this == rhs); }

	const float &operator[](uint i) const {
		SDL_assert(i < 2);
		return *(&x + i);
	}
	float &operator[](uint i) {
		SDL_assert(i < 2);
		return *(&x + i);
	}
	operator const float *() const { return &x; }
};

struct Vector3 {
	float x, y, z;

	float Length() const { return std::sqrt(x * x + y * y + z * z); }

	Vector3 &operator+=(const Vector3 &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3 &operator-=(const Vector3 &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vector3 &operator*=(const Vector3 &rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	Vector3 &operator/=(const Vector3 &rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
	Vector3 &operator+=(float rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}
	Vector3 &operator-=(float rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}
	Vector3 &operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vector3 &operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	Vector3 operator+(const Vector3 &rhs) const {
		Vector3 v(x + rhs.x, y + rhs.y, z + rhs.z);
		return v;
	}
	Vector3 operator-(const Vector3 &rhs) const {
		Vector3 v(x - rhs.x, y - rhs.y, z - rhs.z);
		return v;
	}
	Vector3 operator*(const Vector3 &rhs) const {
		Vector3 v(x * rhs.x, y * rhs.y, z * rhs.z);
		return v;
	}
	Vector3 operator/(const Vector3 &rhs) const {
		Vector3 v(x / rhs.x, y / rhs.y, z / rhs.z);
		return v;
	}
	Vector3 operator+(float rhs) const {
		Vector3 v(x + rhs, y + rhs, z + rhs);
		return v;
	}
	Vector3 operator-(float rhs) const {
		Vector3 v(x - rhs, y - rhs, z - rhs);
		return v;
	}
	Vector3 operator*(float rhs) const {
		Vector3 v(x * rhs, y * rhs, z * rhs);
		return v;
	}
	Vector3 operator/(float rhs) const {
		Vector3 v(x / rhs, y / rhs, z / rhs);
		return v;
	}

	bool operator==(const Vector3 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const Vector3 &rhs) const { return !(*this == rhs); }

	const float &operator[](uint i) const {
		SDL_assert(i < 3);
		return *(&x + i);
	}
	float &operator[](uint i) {
		SDL_assert(i < 3);
		return *(&x + i);
	}
	operator const float *() const { return &x; }
};
