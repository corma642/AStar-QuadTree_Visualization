#pragma once

namespace RenderSpeed
{
	inline float GetRenderSpeed(const int speedNumber)
	{
		switch (speedNumber)
		{
		case 0: return 0.5f;
		case 1: return 0.35f;
		case 2: return 0.275f;
		case 3: return 0.225f;
		case 4: return 0.175f;
		case 5: return 0.10f;
		case 6: return 0.07f;
		case 7: return 0.04f;
		case 8: return 0.02f;
		case 9: return 0.01f;
		case 10: return 0.005f;
		}
		return 0.0f;
	}
}