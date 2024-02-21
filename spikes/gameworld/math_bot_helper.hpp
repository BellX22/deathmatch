#pragma once

#include "the_bot.hpp"
#include "game_world.hpp"
#include <corelib/math/math.hpp>

// INFO: does not return the first in sight instead the first bot who fits the condition
inline
TheBot* get_first_intersecting_bot(v2 from, v2 to, int shooterId, GameWorld* world) {
	TheBot* closest = nullptr;
	for (auto& b : world->get_bots()) {
		if (b->get_id() != shooterId &&
			distance_point_to_line(b->get_pos(), { from, to }) < b->get_bounding_radius())
		{
			closest = b.get();
			break;
		}
	}
	return closest;
}

inline
std::vector<TheBot*> get_intersecting_bots(v2 from, v2 to, int shooterId, GameWorld* world, bool includeshooter = false) {
	std::vector<TheBot*> hits;
	for (auto& b : world->get_bots()) {
		bool intersects = distance_point_to_line(b->get_pos(), { from, to }) < b->get_bounding_radius();
		if (includeshooter && intersects) {
			hits.push_back(b.get());
		}
		else if (b->get_id() != shooterId && intersects) {
			hits.push_back(b.get());
		}
	}
	return hits;
}

inline
TheBot* test_for_bounding_raidus_collision(v2 pos, float radius, int shooterId, GameWorld* world) {
	TheBot* hit = nullptr;
	for (auto& b : world->get_bots()) {
		if (b->get_id() != shooterId &&
			intersection_circle_cirlce({ pos, radius },{ b->get_pos(), b->get_bounding_radius() }))
		{
			return b.get();
		}
	}
	return hit;
}

inline
std::vector<TheBot*> test_for_bounding_raidus_collisions(v2 pos, float radius, int shooterId, GameWorld* world, bool includeshooter = false) {
	std::vector<TheBot*> hits;
	for (auto& b : world->get_bots()) {
		bool intersects = intersection_circle_cirlce({pos, radius}, { b->get_pos(), b->get_bounding_radius() });
		if (includeshooter && intersects) {
			hits.push_back(b.get());
		}
		else if (b->get_id() != shooterId && intersects) {
			hits.push_back(b.get());
		}
	}
	return hits;
}
