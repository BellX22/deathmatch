#pragma once
#include "entity.hpp"

class Trigger : public Entity
{
public:
    auto update(float dt_sec) -> void;
    auto test(Entity const& entity) -> bool;
private:
    bool m_remove_from_game{false};
    bool m_active{true};
    id_t m_graph_node_id{0};
    v2 m_region_pos;
    v2 m_region_size;
};
