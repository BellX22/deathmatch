#pragma once
#include "world.hpp"
#include <SFML/Graphics.hpp>

class DebugRenderer {
public:
	DebugRenderer(sf::RenderTarget& target);
	auto set_view(sf::View view) -> void;
	auto set_center(v2 const& position) -> void;
	auto set_zoom(float zoom) -> void;
	auto draw(World& world) -> void;
	auto handle_event(sf::Event const& event) -> void;
private:
	auto draw_line(v2 const& start, v2 const& end, sf::Color color) -> void;
	auto draw_thick_line(v2 const& start, v2 const& end, float thickness, sf::Color color) -> void;
	auto draw_circle(v2 const& center, float radius, sf::Color color) -> void;
private:
	sf::RenderTarget& m_target;
	sf::View          m_view;
	sf::Text          m_text;
	sf::RectangleShape m_rect;
};
