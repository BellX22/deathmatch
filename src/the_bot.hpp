#pragma once
#include "entity.hpp"
//#include "weapon_system.hpp"
//#include "ai/goal_think.hpp"

class World;

enum class BotStatus {
	Alive,
	Dead,
	Spawing,
};

class TheBot : public Entity {
public:
	using Ptr = std::unique_ptr<TheBot>;

	TheBot(World& world);

	auto shoot_weapon() -> void;

	auto rotate(float rad) -> void;
	auto facing() const -> v2 { return m_facing; }
	auto set_facing(v2 const &facing) -> void { m_facing = facing; }
	auto status() const -> BotStatus { return m_status; }
	auto is_alive() const -> bool { return m_status == BotStatus::Alive; }
	auto is_dead() const -> bool { return m_status == BotStatus::Dead; }
	auto decrease_health(int amount) -> void;
	auto health() -> int { return m_health; }
	auto max_health() -> int { return m_max_health; }

	auto update(sf::Time dt) -> void override final;

private:
	World&    m_world;
	BotStatus m_status{BotStatus::Alive};
	v2        m_facing{1, 0};
	int       m_max_health{100};
	int       m_health{m_max_health};
};
