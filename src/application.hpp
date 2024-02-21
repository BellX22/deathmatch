#pragma once
#include "world.hpp"
#include "frame_counter.hpp"
#include "debug_renderer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application {
public:
	Application(int argc, char **argv);
	auto run() -> void;

private:
	auto process_events() -> void;
	auto update(sf::Time dt) -> void;
	auto render() -> void;

private:
	const sf::Time   m_time_per_frame;
	sf::RenderWindow m_window;
	FrameCounter     m_fps_counter;
	sf::Font         m_default_font;
	sf::Text         m_frame_counter_text;
	World            m_world;
	DebugRenderer    m_debug;
};
