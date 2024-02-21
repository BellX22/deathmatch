#pragma once
#include "the_bot.hpp"
#include "projectile.hpp"
#include "wall.hpp"
#include <vector>

class World {
public:
    World();

    auto handle_player_realtime_input(sf::Time dt) -> void;

    auto update(sf::Time dt) -> void;

    auto add_player() -> void;
    auto add_wall(v2 const& start, v2 const& end) -> void;
    auto add_projectile(EntityType type, int shooter_id, v2 const& pos, v2 const& dir) -> void;
    auto add_spawn_point(v2 const& pos) -> void;

    auto walls() const -> std::vector<Wall::Ptr> const& { return m_walls; }
    auto bots() const -> std::vector<TheBot::Ptr> const& { return m_bots; }
    auto projectiles() const -> std::vector<Projectile::Ptr> const& { return m_projectiles; }
    auto spawn_points() const -> std::vector<v2> const& { return m_spawnpoints; }

    // map is only conceptual...
    //auto load_map(std::istream& in) -> void;
    //auto save_map(std::ostream& out) -> void;
    //auto save() -> void;
    //auto restore() -> void;
    //
    //auto remove_last_added_bot() -> void;
    //auto is_path_obstructed(v2 a, v2 b, float radius) -> bool;
    //auto is_visible(TheBot* viewer, TheBot* viewed) -> bool;
    //auto is_line_of_sight_ok(v2 a, v2 b) -> bool;
    //auto get_dist_to_closest_wall(v2 start, v2 direction) -> float;
    //auto get_pos_of_closest_switch(v2 pos, int door_id) -> v2;
    //auto get_bot_at_position(v2 pos) -> TheBot*;
    //auto get_all_bots_in_fov(TheBot* bot) -> std::vector<TheBot*>;

public:
    bool        m_paused{false};
    sf::Time    m_world_time{};
    int         m_id_counter{0};

    // Objects
    TheBot*                       m_player_bot{nullptr};
    v2                            m_player_start{0, 0};
    std::vector<Wall::Ptr>        m_walls;
    std::vector<TheBot::Ptr>      m_bots;
    std::vector<Projectile::Ptr>  m_created_projectiles;
    std::vector<Projectile::Ptr>  m_projectiles;
    std::vector<v2>               m_spawnpoints;
    //std::vector<Trigger::Ptr>   m_triggers;
    //std::vector<Gravemarker> m_graves;
    //std::vector<Message>     m_messages;

    // Navgraph
    //v2                m_map_min{0, 0};
    //v2                m_map_max{0, 0};
};
