#include "steering_behaviour.hpp"
#include "steering_algorithms.hpp"
#include "entity.hpp"

void SteeringBehaviour::set_target(v2 const& target)
{
	m_target_valid = true;
	m_current_target = target;
}

auto SteeringBehaviour::is_target_valid() const -> bool
{
	return m_target_valid;
}

auto SteeringBehaviour::set_target_valid(bool value) -> void
{
	m_target_valid = value;
}

auto SteeringBehaviour::target() const -> v2
{
	return m_current_target;
}

auto SteeringBehaviour::accumulate_force(
		v2& current_force, 
		v2 const& force_to_add, 
		float max_force) -> bool
{
	auto const magnitude = length(current_force);
	auto const remaining = max_force - magnitude;

	if (remaining <= 0.f)
		return false;

	auto magnitude_to_add = length(force_to_add);

	if (magnitude_to_add < remaining) {
		current_force += force_to_add;
	} else {
		magnitude_to_add = remaining;
		current_force += normalize(force_to_add) * magnitude_to_add;
	}

	return true;
}

auto SteeringBehaviour::calculate_force(Entity const& e) -> v2
{
	v2 force{0.f, 0.f};

	if (m_target_valid)
		force = calculate_prioritized(e);

	return force;
}

auto SteeringBehaviour::calculate_prioritized(Entity const& e) -> v2
{
	v2 total_force{0, 0};

	if ((m_flags & SteeringFlag_Seek)) {
		auto const force = calculate_seek(e) * m_weight_seek;
		if (!accumulate_force(total_force, force, e.max_force()))
			return total_force;
	}

	if ((m_flags & SteeringFlag_Arrive)) {
		auto const force = calculate_arrive(e) * m_weight_arrive;
		if (!accumulate_force(total_force, force, e.max_force()))
			return total_force;
	}

	if ((m_flags & SteeringFlag_Wander)) {
		auto const force = calculate_wander(e) * m_weight_wander;
		if (!accumulate_force(total_force, force, e.max_force()))
			return total_force;
	}

#if 0
	if (m_flags & SteeringType::WallAvoidance) {
		auto const force = wall_avoidance() * m_weight_wall_avoidance;

		if (!accumulate_force(total_force, force))
			return total_force;
	}
#endif
	return total_force;
}

auto SteeringBehaviour::calculate_seek(Entity const& e) -> v2
{
	return compute_seek_force(
			e.pos(), 
			e.velocity(), 
			e.max_speed(), 
			m_current_target);
}


auto SteeringBehaviour::calculate_arrive(Entity const& e) -> v2
{
	return compute_arrive_force(
			e.pos(), 
			e.velocity(), 
			e.max_speed(),
			m_current_target,
			1.0f);
}

auto SteeringBehaviour::calculate_wander(Entity const& e) -> v2
{
	auto jitter = rand_float() * 0.2f;
	auto local_force = compute_local_wander_force(
			m_wander_target,
			m_wander_radius,
			m_wander_distance,
			jitter);
	auto global = body_to_world(e, local_force);
	return global - e.pos();
}

#if 0

auto SteeringBehaviour::calculate_wall_avoidance() -> v2
{

	v2 force(0, 0);
#if 0
	for(auto&& feeler : m_feelers)
	{
		for(auto&& wall : walls)
		{
		}
	}
#endif
#if 0
	for (size_t feeler = 0; feeler < m_feelers.size(); ++feeler) {
		v2 pos = m_feelers[feeler];
		if (map.get_texture_id_block(pos.x, pos.y) != 0) {
			v2 org{ float((int)pos.x), float((int)pos.y) };
			v2 right{ 1.f, 0.f };
			v2 down{ 0.f, 1.f };
			v2 ip{};
			v2 normal{};
			// right line
			if (intersection_line_line({ m_owner->get_pos(), m_feelers[feeler] }, { org + right, org + right + down }, &ip)) {
				normal = { 1.f, 0 };
				v2 overshoot = m_feelers[feeler] - ip;
				steeringForce = normal * length(overshoot);
			}
			// left line
			if (intersection_line_line({ m_owner->get_pos(), m_feelers[feeler] }, { org, org + down }, &ip)) {
				normal = { -1.f, 0 };
				v2 overshoot = m_feelers[feeler] - ip;
				steeringForce = normal * length(overshoot);
			}
			// top line -> y lower
			if (intersection_line_line({ m_owner->get_pos(), m_feelers[feeler] }, { org, org + right }, &ip)) {
				normal = { -1.f, 0 };
				v2 overshoot = m_feelers[feeler] - ip;
				steeringForce = normal * length(overshoot);
			}
			// bot line -> y greater
			if (intersection_line_line({ m_owner->get_pos(), m_feelers[feeler] }, { org + down, org + right + down }, &ip)) {
				normal = { 1.f, 0 };
				v2 overshoot = m_feelers[feeler] - ip;
				steeringForce = normal * length(overshoot);
			}
		}
	}
#endif
	return force;
}

#endif

void SteeringBehaviour::seek_on()
{
	m_flags |= SteeringFlag_Seek;
}

void SteeringBehaviour::seek_off()
{
	if(m_flags & SteeringFlag_Seek)
		m_flags ^= SteeringFlag_Seek;
}

void SteeringBehaviour::arrive_on()
{
	m_flags |= SteeringFlag_Arrive;
}

void SteeringBehaviour::arrive_off()
{
	if (m_flags & SteeringFlag_Arrive)
		m_flags ^= SteeringFlag_Arrive;
}

void SteeringBehaviour::wander_on()
{
	m_flags |= SteeringFlag_Wander;
}

void SteeringBehaviour::wander_off()
{
	if(m_flags & SteeringFlag_Wander)
		m_flags ^= SteeringFlag_Wander;
}

void SteeringBehaviour::wall_avoidance_on()
{
	m_flags |= SteeringFlag_WallAvoidance;
}

void SteeringBehaviour::wall_avoidance_off()
{
	if(m_flags & SteeringFlag_WallAvoidance)
		m_flags ^= SteeringFlag_WallAvoidance;
}

auto SteeringBehaviour::feelers() const -> std::array<v2, 3> const&
{
	return m_feelers;
}

