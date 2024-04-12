#include "entity.hpp"

auto Entity::id() const -> int
{
    return m_id;
}

auto Entity::set_id(int id) -> void
{
    m_id = id;
}

auto Entity::type() const -> EntityType
{
    return m_type;
}

auto Entity::set_type(EntityType type) -> void
{
    m_type = type;
}

auto Entity::destroy() -> void
{
    m_remove_from_game = true;
}

auto Entity::is_destroyed() -> bool
{
    return m_remove_from_game;
}

auto Entity::predicted_pos(sf::Time dt) -> v2
{
	return pos() + velocity() * dt.asSeconds();
}

auto Entity::pos() const -> v2
{
    return m_position;
}

auto Entity::set_pos(v2 const &pos) -> void
{
    m_position = pos;
}

auto Entity::heading() const -> v2
{
    return m_heading;
}

auto Entity::set_heading(const v2 &h) -> void
{
    m_heading = normalize(h);
    m_side = v2{m_heading.y, -m_heading.x};
}

auto Entity::side() const -> v2
{
    return m_side;
}

auto Entity::mass() const -> float
{
    return m_mass;
}

auto Entity::set_mass(float mass) -> void
{
    m_mass = mass;
}

auto Entity::velocity() const -> v2
{
    return m_velocity;
}

auto Entity::set_velocity(const v2 &v) -> void
{
    m_velocity = v;
}

auto Entity::speed() const -> float
{
    return length(m_velocity);
}

auto Entity::max_speed() const -> float
{
    return m_max_speed;
}

auto Entity::set_max_speed(float speed) -> void
{
    m_max_speed = speed;
}

auto Entity::max_force() const -> float
{
    return m_max_force;
}

auto Entity::set_max_force(float force) -> void
{
    m_max_force = force;
}

auto Entity::radius() const -> float
{
	return m_bounding_radius;
}

auto Entity::set_radius(float radius) -> void
{
	m_bounding_radius = radius;
}

auto body_to_world(Entity const& e, v2 const& point) -> v2
{
	/* Vector rotation local -> world
	 * [ xvec.x   yvec.x ] [ x ] = [ xvec.x * x + yvec.x * y]
	 * [ xvec.y   yvec.y ] [ y ] = [ xvec.y * x + yvec.y * y]
	 */
	auto const xvec = e.heading();
	auto const yvec = e.side();
	auto const newx = (xvec.x * point.x + yvec.x * point.y) + e.pos().x;
	auto const newy = (xvec.y * point.x + yvec.y * point.y) + e.pos().y;
	return v2{newx, newy};
}

auto world_to_body(Entity const& e, v2 const& point) -> v2
{
	auto const p = point - e.pos();
	auto const xvec = e.heading();
	auto const yvec = e.side();
	auto const newx = (xvec.x * p.x + xvec.y * p.y);
	auto const newy = (yvec.x * p.x + yvec.y * p.y);
	return v2{newx, newy};
}

auto forward_component(Entity const& e, v2 const& v) -> float
{
	auto const h = e.heading();
	auto const dot = h.x * v.x + h.y * v.y;
	return dot;
}

auto side_component(Entity const& e, v2 const& v) -> float
{
	auto const h = e.side();
	auto const dot = h.x * v.x + h.y * v.y;
	return dot;
}
