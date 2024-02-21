#pragma once
#include "weapon.hpp"
#include <unordered_map>

class TheBot;

class WeaponSystem {
public:
	WeaponSystem(TheBot& owner, World& world);
	auto update(sf::Time dt) -> void; 
	auto shoot() -> void;
	auto select_next_weapon() -> void;
	auto select_previous_weapon() -> void;
	auto has_weapon(WeaponType type) -> bool;
	auto weapon(WeaponType type) -> Weapon&;
	auto current_weapon() -> Weapon&;

private:
	TheBot&                                m_owner;
	std::unordered_map<WeaponType, bool>   m_has_weapon;
	std::unordered_map<WeaponType, Weapon> m_weapons;
	WeaponType                             m_current_weapon;
};
