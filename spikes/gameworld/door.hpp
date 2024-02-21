#pragma once
#include "entity.hpp"
#include "wall.hpp"

enum class DoorState {
    Open,
    Opening,
    Closed,
    Closing,
};

class Door : public Entity
{
public:
    auto set_world(class World* world) -> void { m_world = world; }
    auto set_wall1(index_t wall) -> void;
    auto set_wall2(index_t wall) -> void;
    auto update(float dt_sec) -> void;
    //auto handle_msg(Message const& msg) -> bool;

    auto add_switch(id_t id) -> void;
    auto remove_switch(id_t id) -> void;

    auto get_wall1() const { return m_wall1; }
    auto get_wall2() const { return m_wall2; }

    auto const& get_switches() const { return m_switches; }

    auto open() -> void;

private:
    class World*      m_world{nullptr};
    DoorState         m_state{DoorState::Closed};
    std::vector<id_t> m_switches;
    index_t           m_wall1{InvalidIndex};
    index_t           m_wall2{InvalidIndex};
    Wall              m_wall1_org;
    Wall              m_wall2_org;
    float             m_time_to_open_sec{1};
    float             m_time_to_close_sec{1};
    float             m_time_stay_open_sec{2};
    float             m_timer_sec{0};
    v2                m_normal{0, 0};
    v2                m_max_a{0, 0};
    v2                m_max_b{0, 0};
    float             m_max_size{1};
    float             m_size{0};
};
