#include "projectile.hpp"
#include "the_bot.hpp"
#include "game_config.hpp"
#include "world.hpp"
#include "intersection_tests.hpp"
#include <iostream>

Projectile::Projectile(World& world, int id, v2 const& org, v2 const& dir) 
	: Entity()
	  , m_world{world}
	  , m_shooter_id{id}
	  , m_has_impacted{false}
	  , m_origin{org}
	  , m_impact_point{org}
	  , m_timevisible{}
{
	set_pos(org);
	set_heading(dir);
	set_velocity(max_speed() * heading());
}

void Projectile::update(sf::Time dt)
{
	if (!has_impacted()) {
		auto const delta_pos = velocity() * dt.asSeconds();
		m_distance_traveled += length(delta_pos);
		set_pos(pos() + delta_pos);
		test_for_impact(dt);
	}
	else {
		update_after_impact(dt);
	}
}

ProjectileBolt::ProjectileBolt(World& world, int shooter_id, v2 const& org, v2 const& dir)
	: Projectile(world, shooter_id, org, dir)
{
	set_type(EntityType::ProjectileBolt);
	set_radius(0.f);
	set_max_speed(gameconfig::BOLT_MAX_SPEED);
	set_velocity(max_speed() * heading());
}

////////////////////////////////////////////////////////////////////////////////

auto ProjectileBolt::test_for_impact(sf::Time dt) -> void
{
	if(m_has_impacted)
		return;

	TheBot* hit = get_first_intersecting_bot(
			pos(), 
			pos() + heading() * gameconfig::BOLT_LENGTH,
		       	m_shooter_id,
		       	m_world);

	if (hit && hit->is_alive()) {
		m_has_impacted = true;
		destroy();
		hit->decrease_health(static_cast<int>(gameconfig::BOLT_DMG));
	}

	if(m_distance_traveled > gameconfig::BOLT_RANGE) {
		m_has_impacted = true;
		destroy();
	}

	// TODO@Andreas: check with walls
	for(auto& w : m_world.walls()) {
		v2 ip;
		if(intersection_line_line(w->start(), w->end(), pos(), predicted_pos(dt), ip)) {
			m_has_impacted = true;
			destroy();
			m_impact_point = ip;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

ProjectileRocket::ProjectileRocket(World& world, int shooterId, v2 const& org, v2 const& dir)
	: Projectile(world, shooterId, org, dir)
	, m_current_blast_radius{ 0.f }
	, m_max_blast_radius{ gameconfig::ROCKET_MAX_BLASTRADIUS }
{
	set_type(EntityType::ProjectileRocket);
	set_radius(gameconfig::ROCKET_BOUNDINGRADIUS);
	set_max_speed(gameconfig::ROCKET_MAX_SPEED);
	set_velocity(max_speed() * heading());
}

auto ProjectileRocket::test_for_impact(sf::Time dt) -> void
{
	TheBot* hit = test_for_radii_collision(pos(), radius(), m_shooter_id, m_world);

	if (hit && hit->is_alive()) {
		m_has_impacted = true;
		m_impact_point = pos();
		hit->decrease_health(static_cast<int>(gameconfig::ROCKET_DMG));
		m_bots_hit.insert(hit);
		v2 direction = normalize(hit->pos() - pos());
		hit->set_velocity(hit->velocity() + direction * gameconfig::ROCKET_BLASTRADIUS_PUSHBACK);
		return;
	}

	for(auto& w : m_world.walls()) {
		auto const dist = distance_point_to_line_segment(pos(), w->start(), w->end());
		if(dist< radius()) {
			m_has_impacted = true;
			m_impact_point = pos();
			break;
		}
	}


	if(m_distance_traveled > gameconfig::ROCKET_RANGE) {
		m_has_impacted = true;
		m_impact_point = pos();
	}
}

void ProjectileRocket::update_after_impact(sf::Time dt)
{
	m_current_blast_radius += gameconfig::ROCKET_BLASTRADIUS_SPEED * dt.asSeconds();

	if (m_current_blast_radius >= m_max_blast_radius) {
		destroy();
		return;
	}

	std::vector<TheBot*> hits = test_for_radii_collisions(pos(), m_current_blast_radius, m_shooter_id, m_world);

	for (auto b : hits) {
		if (m_bots_hit.find(b) == m_bots_hit.end() && b->is_alive()) {
			float invradius = (1.f - m_current_blast_radius / m_max_blast_radius);
			b->decrease_health(static_cast<int>(invradius * gameconfig::ROCKET_DMG));
			v2 direction = normalize(b->pos() - pos());
			b->set_velocity(b->velocity() + direction * gameconfig::ROCKET_BLASTRADIUS_PUSHBACK);
			m_bots_hit.insert(b);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
ProjectilePellet::ProjectilePellet(World& world, int shooterId, v2 const& org, v2 const& heading)
	: Projectile(world, shooterId, org, heading)
{
	set_type(EntityType::ProjectilePellet);
	set_radius(0.0f);
	set_max_speed(gameconfig::PELLET_RANGE);
}

auto ProjectilePellet::test_for_impact(sf::Time dt) -> void
{
	m_has_impacted = true;
	m_impact_point = m_origin + heading() * gameconfig::PELLET_RANGE;
	v2 ip;

	auto dist = std::numeric_limits<float>::max();

	for(auto& w : m_world.walls()) {
		if(intersection_line_line(
					w->start(), 
					w->end(), 
					m_origin, 
					m_impact_point, 
					ip)) {

			auto const new_dist = length2(ip - m_origin);
			if(new_dist < dist) {
				dist = new_dist;
			}
		}
	}

	if(dist < gameconfig::PELLET_RANGE * gameconfig::PELLET_RANGE) {
		m_impact_point = m_origin + heading() * std::sqrt(dist);
	}


	auto hit = get_first_intersecting_bot(m_origin, m_impact_point, m_shooter_id, m_world);

	if (hit && hit->is_alive()) {
		hit->decrease_health(static_cast<int>(gameconfig::PELLET_DMG));
		m_impact_point = hit->pos();
	}

	set_pos(m_impact_point);
}

auto ProjectilePellet::update_after_impact(sf::Time dt) -> void
{
	m_timevisible += dt;
	if (m_timevisible.asSeconds() > gameconfig::PELLET_SECS_VISIBLE) {
		destroy();
	}
}


ProjectileSlug::ProjectileSlug(World& world, int shooterId, v2 const& org, v2 const& heading)
	: Projectile(world, shooterId, org, heading)
{
	set_type(EntityType::ProjectileSlug);
	set_radius(0.0f);
	set_max_speed(gameconfig::SLUG_RANGE);
}

auto ProjectileSlug::test_for_impact(sf::Time dt) -> void
{
	m_has_impacted = true;
	m_impact_point = m_origin + heading() * gameconfig::SLUG_RANGE;
	v2 ip;

	auto dist = std::numeric_limits<float>::max();

	for(auto& w : m_world.walls()) {
		if(intersection_line_line(
					w->start(), 
					w->end(), 
					m_origin, 
					m_impact_point, 
					ip)) {

			auto const new_dist = length2(ip - m_origin);
			if(new_dist < dist) {
				dist = new_dist;
			}
		}
	}

	if(dist < gameconfig::SLUG_RANGE * gameconfig::SLUG_RANGE) {
		m_impact_point = m_origin + heading() * std::sqrt(dist);
	}

	auto hits = get_intersecting_bots(m_origin, m_impact_point, m_shooter_id, m_world);

	for (auto b : hits) {
		if (b->is_alive()) 
			b->decrease_health(static_cast<int>(gameconfig::SLUG_DMG));
	}

	if(hits.size() > 0) 
		set_pos(hits.front()->pos());
	else 
		set_pos(m_impact_point);
}

auto ProjectileSlug::update_after_impact(sf::Time dt) -> void
{
	m_timevisible += dt;
	if (m_timevisible.asSeconds() > gameconfig::SLUG_SECS_VISIBLE)
		destroy();
}
