#include "door.hpp"
#include "world.hpp"

auto Door::set_wall1(index_t wall) -> void
{
    m_wall1 = wall;
    m_wall1_org = get(m_world->get_walls(), m_wall1);
}
auto Door::set_wall2(index_t wall) -> void
{
    m_wall2 = wall;
    m_wall2_org = get(m_world->get_walls(), m_wall2);
}

auto Door::update(float dt_sec) -> void
{
    auto& wall1 = get(m_world->get_walls(), m_wall1);
    auto& wall2 = get(m_world->get_walls(), m_wall2);

    switch(m_state)
    {
        break;case DoorState::Opening:
    {
        m_timer_sec += dt_sec;
        auto s1 = m_wall1_org.get_start();
        auto e1 = m_wall1_org.get_end();
        auto s2 = m_wall2_org.get_start();
        auto e2 = m_wall2_org.get_end();
        auto factor = 1.f - (m_timer_sec / m_time_to_open_sec);

        auto f = std::clamp(factor, 0.1f, 1.0f);

        auto ne1 = s1 + (e1 - s1) * f; // new end point of wall 1
        auto ns2 = e2 + (s2 - e2) * f; // new end point of wall 2
        wall1.set_end(ne1);
        wall2.set_start(ns2);

        if(m_timer_sec >= m_time_to_open_sec)
        {
            m_timer_sec = 0;
            m_state = DoorState::Open;
        }
    }

    break;case DoorState::Open:
    {
        m_timer_sec += dt_sec;
        if(m_timer_sec >= m_time_stay_open_sec)
        {
            m_state = DoorState::Closing;
            m_timer_sec = 0;
        }
    }

    break;case DoorState::Closing:
    {
        m_timer_sec += dt_sec;
        auto s1 = m_wall1_org.get_start();
        auto e1 = m_wall1_org.get_end();
        auto s2 = m_wall2_org.get_start();
        auto e2 = m_wall2_org.get_end();
        auto factor = (m_timer_sec / m_time_to_close_sec);

        auto f = std::clamp(factor, 0.1f, 1.0f);

        auto ne1 = s1 + (e1 - s1) * f;
        auto ns2 = e2 + (s2 - e2) * f;
        wall1.set_end(ne1);
        wall2.set_start(ns2);

        if(m_timer_sec >= m_time_to_close_sec)
        {
            m_timer_sec = 0;
            m_state = DoorState::Closed;
        }
    }

    break;case DoorState::Closed:
    {
    // do nothing
    }
    break;default:
        break;
    }
}

auto Door::add_switch(id_t id) -> void
{
    if(std::find(m_switches.begin(), m_switches.end(), id) == m_switches.end())
        m_switches.push_back(id);
}

auto Door::remove_switch(id_t id) -> void
{
    std::erase_if(m_switches, [id](auto&& id_) { return id == id_; });
}

auto Door::open() -> void
{
    if(m_state == DoorState::Closed)
    {
        m_timer_sec = 0;
        m_state = DoorState::Opening;
    }
}
