#include "bases/obj.hpp"
#include "bases/pos.hpp"
#include "math/vec.hpp"
#include <cassert>
#include <iostream>

#define TEST(name) constexpr auto test_##name = [](void)
#define ENDTEST(name)                                                          \
	;                                                                          \
	std::cout << "\nBEGINNING TEST " #name "\n";                               \
	test_##name();                                                             \
	std::cout << "TEST " #name " SUCCEEDED\n";

#define FUNNY_ASSERT(expr)                                                     \
	{                                                                          \
		std::cout << #expr << "\n";                                            \
		assert(bool(expr));                                                    \
	}

void do_tests() {
	TEST(vec_math)
	{
		FUNNY_ASSERT(Vector2(7, 3) + Vector2(2, 5) == Vector2(9, 8))
		FUNNY_ASSERT(Vector2(16, 13) - Vector2(7, 5) == Vector2(9, 8))
		FUNNY_ASSERT(Vector2(3, 2) * Vector2(3, 4) == Vector2(9, 8))
		FUNNY_ASSERT(Vector2(27, 96) / Vector2(3, 12) == Vector2(9, 8))
		std::cout << std::endl;
		FUNNY_ASSERT(Vector2(7, 6) + 2.f == Vector2(9, 8))
		FUNNY_ASSERT(Vector2(16, 13) - 7.f == Vector2(9, 6))
		FUNNY_ASSERT(Vector2(3, 2) * 3.f == Vector2(9, 6))
		FUNNY_ASSERT(Vector2(108, 96) / 12.f == Vector2(9, 8))
		std::cout << std::endl;

		FUNNY_ASSERT(Vector3(7, 3, 0) + Vector3(2, 5, 0) == Vector3(9, 8, 0))
		FUNNY_ASSERT(Vector3(16, 13, 2) - Vector3(7, 5, 1) == Vector3(9, 8, 1))
		FUNNY_ASSERT(Vector3(3, 2, 2) * Vector3(3, 4, 0.5) == Vector3(9, 8, 1))
		FUNNY_ASSERT(Vector3(27, 96, 2) / Vector3(3, 12, 2) == Vector3(9, 8, 1))
		std::cout << std::endl;
		FUNNY_ASSERT(Vector3(7, 3, 0) + 3 == Vector3(10, 6, 3))
		FUNNY_ASSERT(Vector3(16, 13, 2) - 15 == Vector3(1, -2, -13))
		FUNNY_ASSERT(Vector3(3, 2, -3) * 5 == Vector3(15, 10, -15))
		FUNNY_ASSERT(Vector3(27, 96, 2) / 2 == Vector3(13.5, 48, 1))
	}
	ENDTEST(vec_math)

	TEST(vec_ops)
	{
		FUNNY_ASSERT(Vector2(3, 4).Length() == 5.f)
		FUNNY_ASSERT(Vector2(1, 0).Dot(Vector2(0, 1)) == 1.f)
		FUNNY_ASSERT(Vector2(8, 234)[1] == 234.f)
		std::cout << std::endl;
		FUNNY_ASSERT(Vector3(5, 0, 0).Length() == 5.f)
		FUNNY_ASSERT(Vector3(8, 234, 6022.f)[2] == 6022.f)
	}
	ENDTEST(vec_ops)

	TEST(obj)
	{
		FUNNY_ASSERT(Object().mName == "[unnamed]")
		FUNNY_ASSERT(Object("Test").mName == "Test")
		FUNNY_ASSERT(Object().FindByName("Nonexistent") == nullptr)
		FUNNY_ASSERT(Object().CanBecome<Vector2>() == false)
	}
	ENDTEST(obj)

	TEST(pos)
	{
		Positionable a, b;
		a.mTransform.mPos = {3.0, 2.0, 50};
		a.mTransform.mRot = {3.0, 2.0, 50};
		a.mTransform.mScale = {3.0, 2.0, 5};
		b.mTransform.mPos = {3.0, 2.0, 50};
		b.mTransform.mRot = {3.0, 2.0, 50};
		b.mTransform.mScale = {3.0, 2.0, 5};
		b.Reparent(&a);
		FUNNY_ASSERT(b.WorldPos() == Vector3(6, 4, 100))
		FUNNY_ASSERT(b.WorldRot() == Vector3(6, 4, 100))
		FUNNY_ASSERT(b.WorldScl() == Vector3(9, 4, 25))
	}
	ENDTEST(pos)
}
