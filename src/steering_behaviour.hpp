#pragma once
#include "utils.hpp"
#include <array>

class Entity;

using SteeringFlag = int;
enum SteeringFlag_ {
    SteeringFlag_None = 0,
    SteeringFlag_Seek = 1 << 0,
    SteeringFlag_Flee = 1 << 1,
    SteeringFlag_Arrive = 1 << 2,
    SteeringFlag_Wander = 1 << 3,
    SteeringFlag_WallAvoidance = 1 << 4,
    //Separation = 1 << 4,
};

class SteeringBehaviour {
public:
    SteeringBehaviour() = default;

    auto calculate_force(Entity const& e) -> v2;
    auto set_target(v2 const& target) -> void;
    auto target() const -> v2;
    auto is_target_valid() const -> bool;
    auto set_target_valid(bool value) -> void;

    auto wander_target() const -> v2 { return m_wander_target; }
    auto wander_radius() const -> float { return m_wander_radius; }
    auto wander_distance() const -> float { return m_wander_distance; }

    auto is_on() const -> bool { return m_flags; }
    auto all_off() -> void { m_flags = SteeringFlag_None; }
    auto seek_on() -> void;
    auto seek_off() -> void;
    auto arrive_on() -> void;
    auto arrive_off() -> void;
    auto wall_avoidance_on() -> void;
    auto wall_avoidance_off() -> void;
    auto wander_on() -> void;
    auto wander_off() -> void;
    auto feelers() const -> std::array<v2, 3> const&;
    auto feeler_length() const -> float { return m_wall_detection_feeler_length; }

private:
    auto calculate_prioritized(Entity const& e) -> v2;
    auto accumulate_force(v2& steeringForce, v2 const& forceToAdd, float max_force) -> bool;
    auto calculate_seek(Entity const& e) -> v2;
    auto calculate_arrive(Entity const& e) -> v2;
    auto calculate_wander(Entity const& e) -> v2;
    auto calculate_wall_avoidance(Entity const& e) -> v2;

private:
    SteeringFlag      m_flags{SteeringFlag_None};
    v2                m_current_target{0, 0};
    bool              m_target_valid{false};
    std::array<v2, 3> m_feelers{v2{1.5, 1.5}, v2{3, 0}, v2{1.5, -1.5}};
    float             m_wall_detection_feeler_length{1};

private: // weights
    float m_weight_seek{1};
    float m_weight_arrive{1};
    float m_weight_wall_avoidance{1};
    float m_weight_wander{1};
    v2    m_wander_target{1, 0};
    float m_wander_distance{2};
    float m_wander_jitter{0.1};
    float m_wander_radius{3};
};
