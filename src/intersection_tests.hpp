#pragma once
#include "the_bot.hpp"
#include "world.hpp"
#include <iostream>

inline auto 
distance_point_to_line_segment(v2 const& p, v2 const& v, v2 const& w) -> float
{
	// Return minimum distance between line segment vw and point p
	auto const l2 = length2(w-v);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) {
		// v == w case
		return length(v - p);
	}

	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line.
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	// We clamp t from [0,1] to handle points outside the segment vw.
	auto const t = std::max(0.0f, std::min(1.0f, dot(p - v, w - v) / l2));
	auto const projection = v + t * (w - v);  // Projection falls on the segment
	return length(projection - p);
}

inline auto
intersection_circle_cirlce(v2 const& p1, float r1, v2 const& p2, float r2) -> bool
{
	auto const radii = r1 + r2;
	if(length2(p2 - p1) < radii*radii) {
		return true;
	}
	return false;
}

inline auto
intersection_line_circle(v2 const& start, v2 const& end, v2 const& center, float radius) -> bool
{
	auto const dist = distance_point_to_line_segment(center, start, end);
	if(dist <= radius)
		return true;
	return false;
}

inline auto
intersection_line_line(
		v2 const& a, 
		v2 const& b, 
		v2 const& c, 
		v2 const& d, 
		v2& ip
		) -> bool
{
	auto const x1 = a.x;
	auto const x2 = b.x;
	auto const x3 = c.x;
	auto const x4 = d.x;
	auto const y1 = a.y;
	auto const y2 = b.y;
	auto const y3 = c.y;
	auto const y4 = d.y;

	auto t = 0.f;
	auto u = 0.f;
	auto const div = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
	if(div == 0)
		return false;

	{
		auto const nom = (x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4);
		t = nom / div;
		if(t > 1 || t < 0)
			return false;
	}
	{
		auto const nom = (x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3);
		u = -(nom / div);
		if(u > 1 || u < 0)
			return false;
	}

	ip.x = x1 + t * (x2 - x1);
	ip.y = y1 + t * (y2 - y1);

	return true;
}

inline auto
get_first_intersecting_bot(
		v2 const& from, 
		v2 const& to, 
		int shooter_id, 
		World& world
		) -> TheBot*
{
	TheBot* closest = nullptr;
	for (auto& bot : world.bots()) {

		if(bot->id() == shooter_id)
			continue;

		auto const dist = distance_point_to_line_segment(bot->pos(), from, to);

		if(dist < bot->radius()) {
			closest = bot.get();
			break;
		}
	}
	return closest;
}

inline auto
get_intersecting_bots(
		v2 const& from, 
		v2 const& to, 
		int shooter_id, 
		World& world
		) -> std::vector<TheBot*>
{
	std::vector<TheBot*> hits;

	for (auto& b : world.bots()) {

		if(b->id() == shooter_id)
			continue;

		bool intersects = distance_point_to_line_segment(b->pos(), from, to) < b->radius();
		if (intersects) {
			hits.push_back(b.get());
		}
	}
	return hits;
}

inline auto 
test_for_radii_collision(
		v2 const& pos, 
		float radius, 
		int shooter_id, 
		World& world
		) -> TheBot*
{
	TheBot* hit = nullptr;
	for (auto& bot : world.bots()) {

		if(bot->id() == shooter_id)
			continue;

		if(intersection_circle_cirlce(
					pos, radius,
					bot->pos(), bot->radius())) {
			hit = bot.get();
			break;
		}
	}
	return hit;
}

inline auto 
test_for_radii_collisions(
		v2 const& pos, 
		float radius, 
		int shooter_id, 
		World& world
		) -> std::vector<TheBot*>
{
	std::vector<TheBot*> hits;
	for (auto& bot : world.bots()) {

		if(bot->id() == shooter_id)
			continue;

		if(intersection_circle_cirlce( pos, radius, bot->pos(), bot->radius())) {
			hits.push_back(bot.get());
		}
	}
	return hits;
}

