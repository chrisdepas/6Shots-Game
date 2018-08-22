#include "stdafx.h"
#include "Catch.hpp"
#include "Util.h"
#define M_PI 3.141592653f

SCENARIO("Box2D -> SFML Conversions") {
	GIVEN("Box2D Color (0.1, 0.5, 0.9, 1.0)") {
		b2Color color1;
		color1.r = 0.1f; color1.g = 0.5f; color1.b = 0.9f; color1.a = 1.0f;

		THEN("util::toSFMLColor should return (25, 127, 229, 255)") {
			sf::Color sfColor = util::toSFMLColor(color1);
			CHECK(sfColor.r == 25);
			CHECK(sfColor.g == 127);
			CHECK(sfColor.b == 229);
			CHECK(sfColor.a == 255);
		}
	}
	GIVEN("b2Vec2 (101.51f, -10.44f)") {
		b2Vec2 vec1(101.51f, -10.44f);
		THEN("util::toSFMLVec2f should return (101.51f, -10.44f)") {
			sf::Vector2f sfVec = util::toSFMLVec2f(vec1);
			CHECK(sfVec.x == vec1.x);
			CHECK(sfVec.y == vec1.y);
		}
		THEN("util::toSFMLVec2i should return (101, -10)") {
			sf::Vector2i sfVec = util::toSFMLVec2i(vec1);
			CHECK(sfVec.x == 101);
			CHECK(sfVec.y == -10);
		}
	}
}
SCENARIO("ToIntFast performance testing") {
	const int repetitions = 100000;
	const int floatVals = 1024;
	auto floats = std::make_unique<float[]>(floatVals);
	auto ints = std::make_unique<int[]>(floatVals);

	for (int i = 0; i < floatVals; i++) {
		floats[ i ] = (float)std::rand() / (float)std::rand();
		ints[ i ] = (int)(floats[ i ]);
	}

	GIVEN(std::string(std::to_string(repetitions) + " repetitions"))  {
		int result;
		clock_t startTime, fastMethodDelta, castMethodDelta;
		//int msec = diff * 1000 / CLOCKS_PER_SEC;
		//printf("Time taken %d s %d ms", msec / 1000, msec % 1000);
		
		startTime = clock();
		for (int i = 0; i < repetitions; i++) {
			float val = floats[ i%floatVals ];
			__asm FLD val;
			__asm FISTTP result;
			CHECK(result == ints[ i%floatVals ]);
		}
		fastMethodDelta = clock() - startTime;

		startTime = clock();
		for (int i = 0; i < repetitions; i++) {
			float val = floats[ i%floatVals ];
			result = (int)floor(val);
			CHECK(result == ints[ i%floatVals ]);
		}
		castMethodDelta = clock() - startTime;

		INFO("ToIntFast: " << fastMethodDelta << " clocks");
		INFO("Int Cast : " << castMethodDelta << " clocks");
		
		long delta = castMethodDelta - fastMethodDelta;
		CHECK(delta > 0);
		double castMethodTime = castMethodDelta / (float)CLOCKS_PER_SEC;
		double fastMethodTime = fastMethodDelta / (float)CLOCKS_PER_SEC;

		std::ostringstream output;
		output << "Timings:\nFloat to Int Cast: \t" << castMethodTime;
		output << "\nFast Float to Int: \t" << fastMethodTime << "\n";

		output << "\nFloat to int cast is " << (delta < 0 ? -1 * delta : delta);
		output << " clocks  " << (delta < 0 ? "slower" : "faster");

		output << "\nFloat to int cast is " << (delta < 0 ? -1 * delta : delta) / (float)CLOCKS_PER_SEC;
		output << " seconds " << (delta < 0 ? "slower" : "faster");

		std::cout << output.str();
	}
}
SCENARIO("Vector Utility Functions") {
	GIVEN("Vector2f (1.0f, 5.0f)") {
		sf::Vector2f vec(1.0f, 5.0f);
		sf::Vector2f vecUnit = vec::UnitVector(vec);
		float vecLength = vec::Length(vec);

		THEN("vec::LengthSquared should return 26.0") {
			CHECK(vec::LengthSquared(vec) == 26.0f);
		}
		THEN("vec::Length should return sqrt(26.0)") {
			CHECK(vec::Length(vec) == sqrt(26.0f));
		}
		THEN("vec::UnitVector should have length 1.0f and be vector (1/sqrt(26), 5/sqrt(26))") {
			sf::Vector2f unit = vecUnit;
			CHECK(vec::Length(unit) == 1.0f);
			CHECK(unit.x == (1.0f / sqrt(26.0f)));
			CHECK(unit.y == (5.0f / sqrt(26.0f)));
		}
		THEN("vec::LimitLength(vector, 2.0f) should clamp the vector length to 2, and be in the same direction as the original vector") {
			sf::Vector2f cloneVec = vec;
			vec::LimitLength(cloneVec, 2.0f);
			CHECK(vec::Length(cloneVec) == 2.0f);
			CHECK(vec::UnitVector(cloneVec) == vecUnit);
		}
		THEN("vec::LimitLength(vector, 100.0f) should not alter the vector magnitude or direction") {
			sf::Vector2f cloneVec = vec;
			vec::LimitLength(cloneVec, 100.0f);
			CHECK(vec::Length(cloneVec) == vecLength);
			CHECK(vec::UnitVector(cloneVec) == vecUnit);
		}
		THEN("vec::LengthLimited(vector, 0.5f) should return the vector clamped to length 0.5, and be in the same direction as the original vector") {
			sf::Vector2f clampVec = vec::LengthLimited(vec, 0.5f);
			CHECK(vec::Length(clampVec) == 0.5f);
			CHECK(vec::UnitVector(clampVec) == vecUnit);
		}
		THEN("vec::LengthLimited(vector, 32.0f) should not alter the vector magnitude or direction") {
			sf::Vector2f clampVec = vec::LengthLimited(vec, 32.0f);
			CHECK(vec::Length(clampVec) == vecLength);
			CHECK(vec::UnitVector(clampVec) == vecUnit);
		}
		THEN("vec::ScaleVector(vector, 106.0f) should return vector in the same direction with magnitude 106") {
			sf::Vector2f scaleVec = vec::ScaleVector(vec, 106.0f);
			CHECK(vec::Length(scaleVec) == 106.0f);
			CHECK(vec::UnitVector(scaleVec) == vecUnit);
		}
		THEN("vec::ScaleVector(vector, 0.99f) should return vector in the same direction with magnitude 0.99") {
			sf::Vector2f scaleVec = vec::ScaleVector(vec, 0.99f);
			CHECK(vec::Length(scaleVec) == Approx(0.99f));

			scaleVec = vec::UnitVector(scaleVec);
			CHECK(scaleVec.x == Approx(vecUnit.x));
			CHECK(scaleVec.y == Approx(vecUnit.y));
		}
		THEN("vec::Rotate(vector, 2*PI) should return unchanged vector") {
			sf::Vector2f newVec = vec;
			vec::Rotate(newVec, 2 * M_PI);
			CHECK(vec::Length(newVec) == vecLength);

			newVec = vec::UnitVector(newVec);
			CHECK(newVec.x == Approx(vecUnit.x));
			CHECK(newVec.y == Approx(vecUnit.y));
		}
		THEN("vec::Rotate(vector, 2.11185) should return (-4.8, -1.71)") {
			sf::Vector2f newVec = vec;
			vec::Rotate(newVec, 2 * M_PI);
			CHECK(vec::Length(newVec) == vec::Length(vec));

			newVec = vec::UnitVector(newVec);
			CHECK(newVec.x == Approx(vecUnit.x));
			CHECK(newVec.y == Approx(vecUnit.y));
		}
	}
}

template <typename T> void Rotate(sf::Vector2<T>& vector, float fRadian) {
	float cosVal = std::cos(fRadian);
	float sinVal = std::sin(fRadian);
	vector = sf::Vector2<T>(
		(T)(vector.x * cosVal - vector.y * sinVal),
		(T)(vector.y * cosVal + vector.x * sinVal)
		);
}

template <typename T> void Rotated(const sf::Vector2<T>& vector, float fRadian) {
	float cosVal = std::cos(fRadian);
	float sinVal = std::sin(fRadian);
	return sf::Vector2<T>(vector.x * cosVal - vector.y * sinVal,
		vector.y * cosVal + vector.y * sinVal);
}

template <typename T> void RotateDegree(sf::Vector2<T>& vector, float fDegree) {
	float cosVal = std::cos(fDegree);
	float sinVal = std::sin(fDegree);
	vector = sf::Vector2<T>(vector.x * cosVal - vector.y * sinVal,
		vector.y * cosVal + vector.y * sinVal);
}

template <typename T> void RotatedDegree(const sf::Vector2<T>& vector, float fDegree) {
	float cosVal = std::cos(fRadian);
	float sinVal = std::sin(fRadian);
	return sf::Vector2<T>(vector.x * cosVal - vector.y * sinVal,
		vector.y * cosVal + vector.y * sinVal);
}

template <typename T> bool IsNull(const sf::Vector2<T>& vector) {
	return vector.x == 0.0f && vector.y == 0.0f;
}