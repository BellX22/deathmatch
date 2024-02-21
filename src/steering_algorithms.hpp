#pragma once
#include "utils.hpp"
#include "entity.hpp"
#include <span>
#include <cassert>

// Single Behaviours

inline
auto compute_seek_force(
		v2 const& position,
		v2 const& velocity,
		float max_speed, 
		v2 const& target
		) -> v2
{
	auto const to_target = target - position;
	auto const direction = normalize(to_target);
	auto const desired =  direction * max_speed;
	auto const result = desired - velocity;
	return result;
}

inline
auto compute_flee_force(
		v2 const& position, 
		v2 const& velocity, 
		float max_speed, 
		v2 const& target,
		float panic_distance
		) -> v2
{
	auto const from_target = position - target;

	if(length(from_target) > panic_distance)
		return v2{0, 0};

	auto const direction = normalize(from_target);
	auto const desired =  direction * max_speed;
	auto const result = desired - velocity;
	return result;
}

inline
auto compute_arrive_force(
		v2 const& position, 
		v2 const& velocity, 
		float max_speed, 
		v2 const& target,
		float deceleration
		) -> v2
{
	auto const to_target = target - position;
	auto const distance = length(to_target);

	if(distance < 0.1)
		return v2{0, 0};

	auto speed = distance / deceleration;
	speed = std::min(speed, max_speed);
	auto const desired = to_target * speed / distance;

	return desired - velocity;
}

inline
auto compute_pursuit_force(
		v2 const& position,
		v2 const& velocity,
		float max_speed,
		v2 const& target_position,
		v2 const& target_velocity
		) -> v2
{
	auto const to_target = target_position - position;
	auto const relative_heading = dot(velocity, target_velocity);

	if((dot(to_target, velocity) > 0) 
			&& (relative_heading < -0.95)) {
		// if target is hot do not predict just seek to position
		return compute_seek_force(position, velocity, max_speed, target_position);
	}

	auto const look_ahead_time = length(to_target) / (max_speed + length(target_velocity));

	// now seek to predicted position
	return compute_seek_force(position, velocity, max_speed, target_position + target_velocity * look_ahead_time);
}

inline
auto compute_evade_force(
		v2 const& position,
		v2 const& velocity,
		float max_speed,
		v2 const& target_position,
		v2 const& target_velocity
		) -> v2
{
	auto const to_target = target_position - position;
	auto const look_ahead_time = length(to_target) / (max_speed + length(target_velocity));
	return compute_flee_force(
			position, 
			velocity, 
			max_speed, 
			target_position + target_velocity * look_ahead_time,
			std::numeric_limits<float>::max());

}

inline
auto compute_local_wander_force(
		v2& point_on_circle,
		float circle_radius,
		float circle_distance,
		float jitter_rad
		) -> v2
{
	point_on_circle = rotate(point_on_circle, jitter_rad);
	point_on_circle = normalize(point_on_circle);
	auto force = point_on_circle * circle_radius;
	force.x += circle_distance;
	return force;
}

// TBD
inline
auto compute_interpose_force(
		) -> v2
{
	return v2{0, 0};
}

// TBD
inline
auto compute_hide_force(
		) -> v2
{
	assert(false);
	return v2{0, 0};
}

// TBD
inline
auto compute_wall_avoidance_force(
		Entity const& self,
		std::span<class Wall*> walls
		) -> v2
{
	assert(false);
	return v2{0, 0};
}

// TBD
inline
auto compute_obstacle_avoidance_force(
		Entity const& self,
		std::span<Entity*> entities
		) -> v2
{
	assert(false);
	//auto box_length = (speed / max_speed) * min_detection_box_length;
	auto distance_to_closest_obstacle = std::numeric_limits<float>::max();
	Entity* closest_intersecting_obstacle = nullptr;

	for(auto e : entities) {

		// do not test with itsself
		if(&self == e)
			continue;

		auto local = world_to_body(self, e->pos());

		// behind self -> ignore
		if(local.x < 0)
			continue;

		auto expanded_radius = self.radius() + e->radius();

		// not touching in y direction
		if(std::fabs(local.y) > expanded_radius)
			continue;

		auto const cx = local.x;
		auto const cy = local.y;
		auto const sqrt_part = std::sqrt(expanded_radius*expanded_radius - cy * cy);
		auto ip = cx - sqrt_part;
		if(ip <= 0) {
			ip = cx + sqrt_part;
		}

		if(ip < distance_to_closest_obstacle) {
			distance_to_closest_obstacle = ip;
			closest_intersecting_obstacle = e;
		}
	}

	return v2{0, 0};
}

// Group Behaviours

