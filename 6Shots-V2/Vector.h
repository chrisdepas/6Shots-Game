#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>
struct Vector2f
{
	float X;
	float Y;

	static inline Vector2f Null() { return Vector2f(0.0f, 0.0f); }
	Vector2f::Vector2f() {
		X = Y = 0;
	}

	Vector2f::Vector2f(float _X, float _Y)
	{
		X = _X;
		Y = _Y;
	}
	float Vector2f::Length()
	{
		return (float)sqrt(X*X + Y*Y);
	}
	Vector2f Vector2f::operator/(float f)
	{
		return Vector2f(X / f, Y / f);
	}
	Vector2f Vector2f::operator+(Vector2f c)
	{
		return Vector2f(X + c.X, Y + c.Y);
	}
	Vector2f Vector2f::operator-(Vector2f c)
	{
		return Vector2f(X - c.X, Y - c.Y);
	}
	void Vector2f::operator+=(Vector2f c)
	{
		X += c.X;
		Y += c.Y;
	}
	void Vector2f::operator*=(float f)
	{
		X *= f;
		Y *= f;
	}
	Vector2f Vector2f::operator*(float f)
	{
		return Vector2f(X*f, Y*f);
	}
	Vector2f Vector2f::operator*(int i)
	{
		return Vector2f(X*i, Y*i);
	}
	void MakeUnitVector() {
		float len = (float)sqrt(X*X + Y*Y);
		if (len != 0.0f) {
			X /= len;
			Y /= len;
		}
	}
	void LimitLength(int maxLen) {
		float fX = X;
		float fY = Y;

		float len = (float)sqrt(fX*fX + fY*fY);
		if (len > (float)maxLen) {
			fX /= len;
			fY /= len;

			fX *= maxLen;
			fY *= maxLen;
		}
		X = fX;
		Y = fY;
	}
};

struct Vector2i
{
	int X;
	int Y;

	

	Vector2i::Vector2i() {
		X = Y = 0;
	} 
	Vector2i::Vector2i(int _X, int _Y)
	{
		X = _X;
		Y = _Y;
	}
	Vector2i::Vector2i(float _X, float _Y) {
		X = (int)_X;
		Y = (int)_Y;
	}
	void LimitLength(int maxLen) {
		float fX = (float)X;
		float fY = (float)Y;

		float len = (float)sqrt(fX*fX + fY*fY);
		if (len > (float)maxLen) {
			fX /= len;
			fY /= len;

			fX *= maxLen;
			fY *= maxLen;
		}
		X = (int)fX;
		Y = (int)fY;
	}
	void Rotate(float fRadian) {
		float oX = (float)X;
		float oY = (float)Y;
		X = (int)(oX*cos(fRadian) - oY*sin(fRadian));
		Y = (int)(oY*cos(fRadian) + oX*sin(fRadian));
	}
	static Vector2i Vector2i::Zero() {
		return Vector2i(0, 0);
	}
	float Vector2i::Length()
	{
		return (float)sqrt((float)(X*X + Y*Y));
	}
	int Vector2i::LengthSquared()
	{
		return X*X + Y*Y;
	}
	Vector2f Vector2i::UnitVector() {
		float len = Length();
		return Vector2f(X / len, Y / len);
	}
	Vector2i Vector2i::operator/(int factor)
	{
		return Vector2i(X / factor, Y / factor);
	}
	Vector2i Vector2i::operator-(Vector2i c)
	{
		return Vector2i(X - c.X, Y - c.Y);
	}
	Vector2i Vector2i::operator+(Vector2i c)
	{
		return Vector2i(X + c.X, Y + c.Y);
	}
	Vector2i Vector2i::operator*(int factor)
	{
		return Vector2i(X*factor, Y*factor);
	}
	bool Vector2i::operator==(Vector2i c)
	{
		return (c.X == X && c.Y == Y);
	}
	bool Vector2i::operator!=(Vector2i c)
	{
		return (!(c.X == X && c.Y == Y));
	}
	void Vector2i::operator*=(float f)
	{
		X = (int)((float)X * f);
		Y = (int)((float)Y * f);
	}
	void Vector2i::operator*=(int i)
	{
		X *= i;
		Y *= i;
	}
};

#endif