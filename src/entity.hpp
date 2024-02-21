#pragma once
#include "entity_type.hpp"
#include "steering_behaviour.hpp"
#include "utils.hpp"
#include <SFML/System/Time.hpp>
#include <memory>

class Entity {
public:
    using Ptr = std::unique_ptr<Entity>;

    Entity() = default;

    auto id() const -> int;
    auto set_id(int id) -> void;

    auto type() const -> EntityType;
    auto set_type(EntityType type) -> void;

    auto destroy() -> void;
    auto is_destroyed() -> bool;

    auto pos() const -> v2;
    auto set_pos(v2 const& pos) -> void;

    auto predicted_pos(sf::Time dt) -> v2;

    auto heading() const -> v2;
    auto set_heading(v2 const& h) -> void;
    auto side() const -> v2;

    auto radius() const -> float;
    auto set_radius(float radius) -> void;

    auto mass() const -> float;
    auto set_mass(float mass) -> void;

    auto velocity() const -> v2;
    auto set_velocity(const v2 &v) -> void;
    auto speed() const -> float;

    auto max_speed() const -> float;
    auto set_max_speed(float speed) -> void;

    auto max_force() const -> float;
    auto set_max_force(float force) -> void;

    auto steering() -> SteeringBehaviour& { return m_steering; }

    virtual auto update(sf::Time dt) -> void {}
    //virtual auto handle_msg(Message const& msg) -> bool;

protected:
    EntityType  m_type{EntityType::None};
    int         m_id{InvalidId};
    bool        m_remove_from_game{false};
    float       m_bounding_radius{1};
    v2          m_position{0, 0};
    v2          m_heading{1, 0};
    v2          m_side{0, 1};
    v2          m_velocity{0, 0};
    float       m_mass{1.0};
    float       m_max_speed{10.0};
    float       m_max_force{10.0};
    SteeringBehaviour m_steering{};
};

auto body_to_world(Entity const& e, v2 const& point) -> v2;
auto world_to_body(Entity const& e, v2 const& point) -> v2;
auto forward_component(Entity const& e, v2 const& v) -> float;
auto side_component(Entity const& e, v2 const& v) -> float;

