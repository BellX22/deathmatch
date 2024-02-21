#include "the_bot.hpp"
#include "world.hpp"

TheBot::TheBot(World& world)
	: m_world{world}
{
	set_type(EntityType::Bot);
}

auto TheBot::rotate(float rad) -> void
{
	set_facing(::rotate(facing(), rad));
}

auto TheBot::update(sf::Time dt) -> void 
{
	v2 force{0, 0};
	if(is_alive()) {
		force = steering().calculate_force(*this);
	}

	// no force applied, apply friction instead
	if(force == v2(0, 0)) {
		constexpr auto Friction = 0.9f;
		m_velocity *= Friction;
	}

	// wall collision
	for(auto& w : m_world.walls()) {
	}

	auto accel = force / mass();
	m_velocity += accel * dt.asSeconds();
	m_velocity = truncate(m_velocity, max_speed());
	m_position += m_velocity * dt.asSeconds();

	if(length(m_velocity) > 0.01) {
		set_heading(m_velocity);
	}
}

auto TheBot::decrease_health(int amount) -> void
{
	m_health -= amount;
	if(m_health < 1) {
		m_status = BotStatus::Dead;
	}
}

auto TheBot::shoot_weapon() -> void
{
	m_world.add_projectile(EntityType::ProjectileBolt, id(), pos(), facing());
	//m_world.add_projectile(EntityType::ProjectileRocket, id(), pos(), facing());
	//m_world.add_projectile(EntityType::ProjectilePellet, id(), pos(), facing());
	//m_world.add_projectile(EntityType::ProjectileSlug, id(), pos(), facing());
}

