#pragma once

namespace Maths
{
	float Lerp(float a, float b, float t);
	float Normalize(float value, float min, float max);
	float Unnormalize(float value, float min, float max);
	float Clamp(float value, float min, float max);
	float Abs(float value);
}