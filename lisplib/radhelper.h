#pragma once

static constexpr double RPI = 3.14159265358979323846;   // pi
inline constexpr float RAD2DEG(float rad)
{
	return (float)(rad * 180.0 / RPI);
}

inline constexpr float DEG2RAD(float deg)
{
	return (float)(deg * RPI / 180.);
}
