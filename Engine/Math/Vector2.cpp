#include "Vector2.h"

#include <iostream>

// 전역 변수 초기화.
Vector2 Vector2::Zero = Vector2(0, 0);
Vector2 Vector2::One = Vector2(1, 1);
Vector2 Vector2::Up = Vector2(1, 0);
Vector2 Vector2::Right = Vector2(0, 1);

Vector2::Vector2(int y, int x)
	:y(y), x(x)
{
}

Vector2::~Vector2()
{
	if (value)
	{
		SafeDelete(value);
	}
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

bool Vector2::operator==(const Vector2& other) const
{
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
	return x != other.x || y != other.y;
}

Vector2::operator COORD()
{
	COORD coord;
	coord.X = static_cast<short>(this->x);
	coord.Y = static_cast<short>(this->y);
	return coord;
}

Vector2::operator COORD() const
{
	COORD coord;
	coord.X = static_cast<short>(this->x);
	coord.Y = static_cast<short>(this->y);
	return coord;
}
