#include "debug_renderer.hpp"
#include "game_config.hpp"
#include <cassert>
#include <sstream>

DebugRenderer::DebugRenderer(sf::RenderTarget& target)
	: m_target{target}
	, m_view{m_target.getDefaultView()}
{
	m_font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSerif.ttf");
	m_text.setFont(m_font);
	m_text.setCharacterSize(18);
	m_text.setFillColor(sf::Color::White);
	m_text.setOutlineColor(sf::Color(50, 50, 50));
	m_text.setOutlineThickness(2.f);
}

auto DebugRenderer::set_view(sf::View view) -> void
{
	m_view = view;
}

auto DebugRenderer::set_center(v2 const& position) -> void
{
	m_view.setCenter(position);
}

auto DebugRenderer::set_zoom(float zoom) -> void
{
	m_view.zoom(zoom);
}

auto DebugRenderer::draw_thick_line(v2 const& from, v2 const& to, float thickness, sf::Color color) -> void
{
    sf::RectangleShape line;
    auto dir = to - from;
    auto dist = length(dir);
    line.setSize({dist, thickness});
    line.setOrigin({0, thickness / 2});
    line.setPosition({from.x, -from.y});
    line.rotate((-std::atan2(dir.y, dir.x)) * 180 / M_PI);
    line.setFillColor(color);
    m_target.draw(line);
}

auto DebugRenderer::draw_line(v2 const& from, v2 const& to, sf::Color color) -> void
{
    sf::Vertex line[2];
    line[0].position = v2(from.x, -from.y);
    line[0].color = color;
    line[1].position = v2(to.x, -to.y);
    line[1].color = color;
    m_target.draw(line, 2, sf::Lines);
}

auto DebugRenderer::draw_circle(v2 const& pos, float radius, sf::Color color) -> void
{
    sf::CircleShape circle;
    circle.setPosition({pos.x, -pos.y});
    circle.setRadius(radius);
    circle.setOrigin({radius, radius});
    circle.setFillColor(color);
    circle.setOutlineColor(sf::Color(100, 100, 100));
    circle.setOutlineThickness(0.2f);
    m_target.draw(circle);
}

auto DebugRenderer::draw_hud(World& world) -> void
{
	std::stringstream ss;
	if(auto player = world.player(); player) {
		auto& weapon = player->weapon_system().current_weapon();
		ss << "Health: " << player->health() << std::endl;
		ss << "Ammo: " << weapon.num_rounds() << " / " << weapon.max_rounds() << std::endl;
		m_text.setString(ss.str());
		m_text.setPosition({10, 50});
		m_target.draw(m_text);
	}
}

auto DebugRenderer::draw(World& world) -> void
{
	auto backup_view = m_target.getView();
	m_target.setView(m_view);
	draw_line({0, 0}, {1, 0}, sf::Color::Red);
	draw_line({0, 0}, {0, 1}, sf::Color::Green);

	for(auto& w : world.walls()) {
		draw_line(w->start(), w->end(), sf::Color::White);
		draw_line(w->center(), w->center() + w->normal(), sf::Color::White);
	}

	for(auto& pos : world.spawn_points()) {
		draw_circle(pos, 1.f, sf::Color(150, 150, 150));
	}

	for(auto& bot : world.bots()) {
		TheBot& e = *bot;
		if(e.is_alive()) {
			draw_circle(e.pos(), 1, sf::Color::Green);
			draw_thick_line(e.pos(), e.pos() + e.heading(), 0.3, sf::Color::Blue);
			draw_thick_line(e.pos(), e.pos() + e.facing(), 0.2, sf::Color::Yellow);
			auto& feelers = e.steering().feelers();
			for(auto& p : feelers) {
				auto pf = body_to_world(e, p * e.steering().feeler_length());
				draw_line(e.pos(), pf, sf::Color::Red);
			}
		} else if(e.is_dead()) {
			draw_circle(e.pos(), 1, sf::Color(165, 42, 42));
		}

		// wander debug
		//auto radius = e.steering().wander_radius();
		//auto p = v2(e.steering().wander_distance(), 0);
		//draw_circle(m_target, body_to_world(e, p), radius, sf::Color::White);
		//auto t = body_to_world(e, e.steering().wander_target() * radius + p);
		//draw_circle(target, t, 0.2, sf::Color::Blue);
	}

	for(auto& p : world.projectiles()) {
		switch(p->type()) {
			case EntityType::ProjectileBolt:
				draw_line(p->pos(), p->pos() + p->heading() * gameconfig::BOLT_LENGTH, sf::Color::Green);
				break;
			case EntityType::ProjectileRocket:
				{
				auto proj = static_cast<ProjectileRocket*>(p.get());
				draw_circle(p->pos(), p->radius(), sf::Color::Green);
				draw_circle(p->pos(), proj->current_blast(), sf::Color::Red);
				}
				break;
			case EntityType::ProjectilePellet:
				{
				auto proj = static_cast<ProjectilePellet*>(p.get());
				draw_line(proj->origin(), proj->impact_point(), sf::Color::Blue);
				}
				break;
			case EntityType::ProjectileSlug:
				{
				auto proj = static_cast<ProjectileSlug*>(p.get());
				draw_line(proj->origin(), proj->impact_point(), sf::Color::Yellow);
				}
				break;
			default:
				assert(false);
		}
	}

	m_target.setView(backup_view);

	draw_hud(world);
}

auto DebugRenderer::handle_event(sf::Event const& event) -> void
{
	if(event.type != sf::Event::KeyPressed)
		return;

	switch(event.key.code) {
		case sf::Keyboard::Left:
			m_view.move({-1, 0});
			break;
		case sf::Keyboard::Right:
			m_view.move({1, 0});
			break;
		case sf::Keyboard::Up:
			m_view.move({0, -1});
			break;
		case sf::Keyboard::Down:
			m_view.move({0, 1});
			break;
		case sf::Keyboard::PageUp:
			m_view.zoom(0.5);
			break;
		case sf::Keyboard::PageDown:
			m_view.zoom(2.0);
			break;
		default:
			break;
	}
}
