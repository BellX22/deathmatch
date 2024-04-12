#include "weapon.hpp"
#include "world.hpp"
#include "game_config.hpp"
#include <cassert>

Weapon::Weapon(TheBot& owner, World& world, WeaponType type) 
	: m_owner{owner}
	, m_world{world}
{
	switch (type) {
	case WeaponType::Blaster:
		m_max_rounds = gameconfig::BLASTER_MAXROUNDS;
		m_num_rounds = gameconfig::BLASTER_MAXROUNDS;
		m_rate_of_fire = sf::seconds(gameconfig::BLASTER_FIRERATE);
		break;
	case WeaponType::Shotgun:
		m_max_rounds = gameconfig::SHOTGUN_MAXROUNDS;
		m_num_rounds = gameconfig::SHOTGUN_MAXROUNDS;
		m_rate_of_fire = sf::seconds(gameconfig::SHOTGUN_FIRERATE);
		break;
	case WeaponType::RocketLauncher:
		m_max_rounds = gameconfig::ROCKETLAUNCHER_MAXROUNDS;
		m_num_rounds = gameconfig::ROCKETLAUNCHER_MAXROUNDS;
		m_rate_of_fire = sf::seconds(gameconfig::ROCKETLAUNCHER_FIRERATE);
		break;
	case WeaponType::Railgun:
		m_type = type;
		m_max_rounds = gameconfig::RAILGUN_MAXROUNDS;
		m_num_rounds = gameconfig::RAILGUN_MAXROUNDS;
		m_rate_of_fire = sf::seconds(gameconfig::RAILGUN_FIRERATE);
		break;
	default:
		assert(false);
		break;
	}
}

auto Weapon::update(sf::Time dt) -> void
{
	m_timer_until_next_shot_available += dt;
}

auto Weapon::shoot() -> void
{
	if (m_num_rounds <= 0) 
		return;

	if (is_ready_to_shoot()) {
		fire_projectile(m_type);
	}
}

auto Weapon::increment_rounds(int num) -> void
{
	m_num_rounds = std::min(m_num_rounds + num, m_max_rounds);
}

auto Weapon::decrement_rounds(int num) -> void
{
	m_num_rounds = std::max(m_num_rounds - num, 0);
}

auto Weapon::is_ready_to_shoot() -> bool
{
	if (m_timer_until_next_shot_available.asSeconds() > (1.f / m_rate_of_fire.asSeconds())) {
		m_timer_until_next_shot_available = sf::seconds(0.f);
		return true;
	}
	return false;
}

auto Weapon::fire_projectile(WeaponType type) -> void
{
	auto const id = m_owner.id();
	auto const pos = m_owner.pos() + m_owner.facing() * m_owner.radius();
	auto const dir = m_owner.facing();

	switch (type) {
	case WeaponType::Blaster:
		decrement_rounds(1);
		m_world.add_projectile(EntityType::ProjectileBolt, id, pos, dir);
		break;
	case WeaponType::RocketLauncher:
		decrement_rounds(1);
		m_world.add_projectile(EntityType::ProjectileRocket, id, pos, dir);
		break;
	case WeaponType::Railgun:
		decrement_rounds(1);
		m_world.add_projectile(EntityType::ProjectileSlug, id, pos, dir);
		break;
	case WeaponType::Shotgun: 
		{
			int numShots = 3;
			float openingAngle = 10.f;
			float angleStep = openingAngle / ((float)numShots - 1);
			float angleStart = -openingAngle / 2.f;
			float angle = angleStart;
			for (int i = 0; i < numShots; i++) {
				if (m_num_rounds > 0) {
					decrement_rounds(1);
					m_world.add_projectile(
							EntityType::ProjectilePellet,
							id, 
							pos,
							rotate(dir, to_radians(angle))); 
				}
				angle += angleStep;
			}
		}
		break;
	default:
		assert(false);
		break;
	}
}
