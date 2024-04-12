#pragma once
#include <SFML/System/Time.hpp>

class TheBot;
class World;

enum class WeaponType {
	Blaster,
	Shotgun,
	RocketLauncher,
	Railgun
};

class Weapon {
public:
	Weapon(TheBot& owner, World& world, WeaponType type);
	auto update(sf::Time dt) -> void;
	auto shoot() -> void;
	auto num_rounds() const -> int { return m_num_rounds; }
	auto set_num_rounds(int num) -> void;
	auto increment_rounds(int num) -> void;
	auto decrement_rounds(int num) -> void;
	auto max_rounds() const -> int { return m_max_rounds; }
	auto type() const -> WeaponType;
private:
	auto is_ready_to_shoot() -> bool;
	auto fire_projectile(WeaponType type) -> void;
private:
	TheBot&    m_owner;
	World&     m_world;
	WeaponType m_type{WeaponType::Blaster};
	int        m_num_rounds{0};
	int        m_max_rounds{0};
	sf::Time   m_rate_of_fire{sf::seconds(1)};
	sf::Time   m_timer_until_next_shot_available{sf::seconds(0)};
};
