#pragma once
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cmath>

using v2 = sf::Vector2f;
constexpr int InvalidIndex = -1;
constexpr int InvalidId = -1;

inline auto to_radians(float deg) -> float
{
	return deg / 180 * M_PI;
}

inline auto length2(v2 const& v) -> float
{
    return v.x*v.x + v.y*v.y;
}

inline auto length(v2 const& v) -> float
{
    return std::sqrt(v.x*v.x + v.y*v.y);
}

inline auto normalize(v2 const& v) -> v2
{
    return v / length(v);
}

inline auto dot(v2 const& a, v2 const& b) -> float
{
    return a.x * b.x + a.y * b.y;
}

inline auto perp(v2 const& v) -> v2
{
	return v2{-v.y, v.x};
}

inline auto reflect(v2 const& v, v2 const& normal) -> v2
{
	auto const n = normalize(normal);
	auto const r = v - 2 * (dot(v, n)) * n;
	return r;
}

inline auto truncate(v2 const& v, float len) -> v2
{
	auto l = length(v);
	if(l < len)
		return v;

	auto result = normalize(v);
	return result * len;
}

inline auto rotate(v2 const& v, float rad) -> v2
{
	// create rotation matrix and then transform vector
	auto const c = std::cos(rad);
	auto const s = std::sin(rad);
	auto const x = c * v.x - s * v.y;
	auto const y = s * v.x + c * v.y;
	return v2{x, y};
}

inline auto rand_float() -> float
{
    float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
    return r;
}

