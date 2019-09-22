#include "Maths.h"

namespace Maths
{
	float Lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	float Normalize(float value, float min, float max)
	{
		float normalized = (value - min) / (max - min);
		return normalized;
	}

	float Unnormalize(float value, float min, float max)
	{
		float unnormalized = (value * (max - min) + min);
		return unnormalized;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	float Abs(float value)
	{
		if (value > 0)
			return value;
		else
			return -value;
	}
}