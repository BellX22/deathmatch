#include "application.hpp"
#include "debug_renderer.hpp"
#include <SFML/Window/Event.hpp>

Application::Application(int argc, char **argv)
	: m_time_per_frame(sf::seconds(1.0f / 30.0f))
	, m_window()
	, m_debug(m_window)
{
	m_window.create(sf::VideoMode::getDesktopMode(), "Deathmatch");
	//m_window.create(sf::VideoMode(640, 480), "Deathmatch");
	m_window.setVerticalSyncEnabled(true);

	m_default_font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSerif.ttf");
	m_frame_counter_text.setFont(m_default_font);
	m_frame_counter_text.setCharacterSize(18);
	m_frame_counter_text.setString("FPS: ");
	m_frame_counter_text.setFillColor(sf::Color::White);
	m_frame_counter_text.setOutlineColor(sf::Color(50, 50, 50));
	m_frame_counter_text.setOutlineThickness(2.f);

	m_debug.set_view(m_window.getDefaultView());
	m_debug.set_center({0, 0});
	m_debug.set_zoom(0.150);
}

auto Application::process_events() -> void
{
    sf::Event event;
    while (m_window.pollEvent(event)) {

        if(event.type == sf::Event::Resized) {
            auto const w = event.size.width;
            auto const h = event.size.height;
        }

        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::KeyPressed 
			&& event.key.code == sf::Keyboard::Escape)
            m_window.close();

        if(event.type == sf::Event::KeyPressed)
            ;//m_world.on_key_press(event.key.code);

	m_debug.handle_event(event);
    }
}

auto Application::update(sf::Time dt) -> void
{
    m_world.update(dt);
}

auto Application::render() -> void
{
	m_debug.draw(m_world);

	m_window.setView(m_window.getDefaultView());
	m_window.draw(m_frame_counter_text);

    //sf::RectangleShape xline({1, 0.1});
    //xline.setFillColor(sf::Color::Red);
    //sf::RectangleShape yline({0.1, -1.0});
    //yline.setFillColor(sf::Color::Green);

    //m_window.draw(xline);
    //m_window.draw(yline);
    //m_world.debug_draw(m_window);
    //auto& fb = m_renderer.fb();
    //m_renderer.draw_world(m_gameworld);
    //float fbw = fb.get_width();
    //float fbh = fb.get_height();
    //auto& color = fb.get_color_buffer();
    //fb.draw_text(5, 5, "FPS: " + std::to_string(m_fps_counter.get_framerate()));
    //m_texture.update(reinterpret_cast<const std::uint8_t*>(color.data()));
    //sf::Sprite sprite(m_texture);
    //auto sx = (float)m_window.getSize().x / fbw;
    //auto sy = (float)m_window.getSize().y / fbh;
    //sprite.setScale({sx, sy});
    //m_window.draw(sprite);
}

auto Application::run() -> void
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (m_window.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > m_time_per_frame) {
            timeSinceLastUpdate -= m_time_per_frame;

            process_events();
            update(m_time_per_frame);
            m_fps_counter.update();
	    m_frame_counter_text.setString(std::string("FPS: ") + std::to_string(m_fps_counter.framerate()));
        }

        m_window.clear();
        render();
        m_window.display();
    }
}
