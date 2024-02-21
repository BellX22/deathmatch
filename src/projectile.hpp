#pragma once
#include "entity.hpp"
#include <unordered_set>

class TheBot;
class World;

class Projectile : public Entity {
public:
	using Ptr = std::unique_ptr<Projectile>;

	auto update(sf::Time dt) -> void override final;
	auto has_impacted() const -> bool { return m_has_impacted; }
	auto impact_point() const -> v2 const& { return m_impact_point; }
	auto origin() const -> v2 const& { return m_origin; }

protected:
	Projectile(World& world, int shooter_id, v2 const& org, v2 const& dir);
	virtual auto test_for_impact(sf::Time dt) -> void = 0;
	virtual auto update_after_impact(sf::Time dt) -> void {}

protected:
	World&   m_world;
	int      m_shooter_id{InvalidId};
	bool     m_has_impacted{false};
	v2       m_origin{0, 0};
	v2       m_impact_point{0, 0};
	float    m_distance_traveled{0};
	sf::Time m_timevisible{};
};

////////////////////////////////////////////////////////////////////////////////
// Blaster
class ProjectileBolt : public Projectile {
public:
	ProjectileBolt(World& world, int shooter_id, v2 const& org, v2 const& dir);
private:
	auto test_for_impact(sf::Time dt) -> void override;
	auto update_after_impact(sf::Time dt) -> void override {}
};

////////////////////////////////////////////////////////////////////////////////
// Rocket
class ProjectileRocket : public Projectile {
public:
	ProjectileRocket(World& world, int shooterId, v2 const& org, v2 const& dir);
	auto current_blast() const -> float { return m_current_blast_radius; }
private:
	auto test_for_impact(sf::Time dt) -> void override;
	auto update_after_impact(sf::Time dt) -> void override;
	float m_current_blast_radius;
	float m_max_blast_radius;
	std::unordered_set<TheBot*> m_bots_hit;
};

////////////////////////////////////////////////////////////////////////////////
// Shotgun
class ProjectilePellet : public Projectile {
public:
	ProjectilePellet(World& world, int shooterId, v2 const& org, v2 const& heading);
private:
	auto test_for_impact(sf::Time dt) -> void override;
	auto update_after_impact(sf::Time dt) -> void override;
};

////////////////////////////////////////////////////////////////////////////////
// Railgun
class ProjectileSlug : public Projectile {
public:
	ProjectileSlug(World& world, int shooterId, v2 const& org, v2 const& heading);

private:
	auto test_for_impact(sf::Time dt) -> void override;
	auto update_after_impact(sf::Time dt) -> void override;
};

