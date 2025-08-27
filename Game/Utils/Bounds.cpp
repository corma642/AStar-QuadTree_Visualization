#include "Bounds.h"

bool Bounds::Intersects(const Bounds& other) const
{
	// AABB 충돌 확인
	// 다른 영역이 현재 영역의 오른쪽 바깥인지 확인
	if (other.GetX() > MaxX())
	{
		return false;
	}

	// 다른 영역이 현재 영역의 왼쪽 바깥인지 확인
	if (other.MaxX() < x)
	{
		return false;
	}

	// 다른 영역이 현재 영역의 아래쪽 바깥인지 확인
	if (other.GetY() > MaxY())
	{
		return false;
	}

	// 다른 영역이 현재 영역의 위쪽 바깥인지 확인
	if (other.MaxY() < y)
	{
		return false;
	}

	return true;
}
