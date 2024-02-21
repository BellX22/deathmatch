#include "world.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cassert>

World::World()
{
	auto const count = 1;
	auto const off = 2.5f;
	for(auto i = 0; i < count; i++) {
		auto bot = std::make_unique<TheBot>(*this);
		auto yoff = i * off - (count * off / 2);
		bot->set_id(++m_id_counter);
		bot->set_pos({10, yoff});
		bot->set_heading({-1, 0});
		bot->steering().set_target({-10, 0});
		//bot->steering().wander_on();
		m_bots.push_back(std::move(bot));
	}

	auto const outer = 25.f;
	auto const inner = 5.f;
	// outer
	add_wall(v2{outer, outer}, v2{-outer, outer});
	add_wall(v2{-outer, outer}, v2{-outer, -outer});
	add_wall(v2{-outer, -outer}, v2{outer, -outer});
	add_wall(v2{outer, -outer}, v2{outer, outer});
	// inner
	add_wall(v2{inner, inner}, v2{inner, -inner});
	add_wall(v2{inner, -inner}, v2{-inner, -inner});
	add_wall(v2{-inner, -inner}, v2{-inner, inner});
	add_wall(v2{-inner, inner}, v2{inner, inner});

	add_spawn_point({-10, 15});
	m_player_start = {-10, 15};
	add_player();

	//add_projectile(EntityType::ProjectileBolt, 0, {0, -1}, {1, 0});
	//add_projectile(EntityType::ProjectileBolt, 0, {0, -2}, {1, 0});
	//add_projectile(EntityType::ProjectileBolt, 0, {0, -3}, {1, 0});
	//add_projectile(EntityType::ProjectileRocket, 0, {0, -4}, {1, 0});
	//add_projectile(EntityType::ProjectileRocket, 0, {0, -1}, {1, 0});
}

auto World::update(sf::Time dt) -> void
{
    if(m_paused)
        return;

    handle_player_realtime_input(dt);

    for(auto& bot : m_bots) {
	    bot->update(dt);
    }

    // merge projectiles
    for(auto&& p : m_created_projectiles) {
	    m_projectiles.push_back(std::move(p));
    }
    m_created_projectiles.clear();


    if(m_player_bot && m_player_bot->is_dead())
	    m_player_bot = nullptr;

    for(auto& p : m_projectiles) {
	    p->update(dt);
    }

    m_projectiles.erase(std::remove_if(
			    m_projectiles.begin(), 
			    m_projectiles.end(), [](auto& proj) { return proj->is_destroyed(); }),
		    m_projectiles.end());

    m_world_time += dt;
}

auto World::add_wall(v2 const& start, v2 const& end) -> void
{
	m_walls.push_back(std::make_unique<Wall>(start, end));
}

auto World::add_spawn_point(v2 const& pos) -> void
{
	m_spawnpoints.push_back(pos);
}

auto World::add_projectile(EntityType type, int shooter_id, v2 const& pos, v2 const& dir) -> void
{
	Projectile::Ptr projectile;
	switch(type) {
		case EntityType::ProjectileBolt:
			projectile = std::make_unique<ProjectileBolt>(*this, shooter_id, pos, dir);
			break;
		case EntityType::ProjectileRocket:
			projectile = std::make_unique<ProjectileRocket>(*this, shooter_id, pos, dir);
			break;
		case EntityType::ProjectileSlug:
			projectile = std::make_unique<ProjectileSlug>(*this, shooter_id, pos, dir);
			break;
		case EntityType::ProjectilePellet:
			projectile = std::make_unique<ProjectilePellet>(*this, shooter_id, pos, dir);
			break;
		default:
			assert(false);
	}

	projectile->set_id(++m_id_counter);
	m_created_projectiles.push_back(std::move(projectile));
}

auto World::add_player() -> void
{
	if(m_player_bot)
		return;

	auto bot = std::make_unique<TheBot>(*this);
	m_player_bot = bot.get();
	bot->set_id(++m_id_counter);
	bot->set_pos(m_player_start);
	bot->set_heading({-1, 0});
	m_bots.push_back(std::move(bot));
}

auto World::handle_player_realtime_input(sf::Time dt) -> void
{
	if(m_player_bot == nullptr)
		return;

	auto const speed = m_player_bot->max_speed();
	auto const pos = m_player_bot->pos();
	v2 target = pos;

	m_player_bot->steering().seek_off();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		target += speed * perp(m_player_bot->facing());
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		target += speed * -perp(m_player_bot->facing());
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		target += speed * m_player_bot->facing();
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		target += speed * -m_player_bot->facing();
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		m_player_bot->rotate(dt.asSeconds());
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		m_player_bot->rotate(-dt.asSeconds());
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		m_player_bot->shoot_weapon();
	}

	m_player_bot->steering().set_target(target);

	if(length(target - pos) > 0.1)
		m_player_bot->steering().seek_on();
}
