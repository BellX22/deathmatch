#include "weapon_system.hpp"
#include <cassert>

WeaponSystem::WeaponSystem(TheBot& owner, World& world) 
	: m_owner{ owner }
	, m_has_weapon{}
	, m_weapons{}
	, m_current_weapon{WeaponType::Blaster}
{
	m_weapons.emplace(std::make_pair(WeaponType::Blaster,        Weapon{owner, world, WeaponType::Blaster} ));
	m_weapons.emplace(std::make_pair(WeaponType::Shotgun,        Weapon{owner, world, WeaponType::Shotgun} ));
	m_weapons.emplace(std::make_pair(WeaponType::RocketLauncher, Weapon{owner, world, WeaponType::RocketLauncher} ));
	m_weapons.emplace(std::make_pair(WeaponType::Railgun,        Weapon{owner, world, WeaponType::Railgun} ));
	m_has_weapon[WeaponType::Blaster] = true;
	m_has_weapon[WeaponType::Shotgun] = false;
	m_has_weapon[WeaponType::RocketLauncher] = false;
	m_has_weapon[WeaponType::Railgun] = false;
}

auto WeaponSystem::update(sf::Time dt) -> void
{
	for (auto& [type, w] : m_weapons) {
		w.update(dt);
	}
}

auto WeaponSystem::shoot() -> void
{
	m_weapons.at(m_current_weapon).shoot();
}

auto WeaponSystem::select_next_weapon() -> void
{
	switch (m_current_weapon) {
	case WeaponType::Blaster:
		m_current_weapon = WeaponType::Shotgun;
		break;
	case WeaponType::Shotgun:
		m_current_weapon = WeaponType::RocketLauncher;
		break;
	case WeaponType::RocketLauncher:
		m_current_weapon = WeaponType::Railgun;
		break;
	case WeaponType::Railgun:
		m_current_weapon = WeaponType::Blaster;
		break;
	}
}

auto WeaponSystem::select_previous_weapon() -> void
{
	switch (m_current_weapon) {
	case WeaponType::Blaster:
		m_current_weapon = WeaponType::Railgun;
		break;
	case WeaponType::Shotgun:
		m_current_weapon = WeaponType::Blaster;
		break;
	case WeaponType::RocketLauncher:
		m_current_weapon = WeaponType::Shotgun;
		break;
	case WeaponType::Railgun:
		m_current_weapon = WeaponType::RocketLauncher;
		break;
	}
}

auto WeaponSystem::weapon(WeaponType type) -> Weapon&
{
	return m_weapons.at(type);
}

auto WeaponSystem::current_weapon() -> Weapon&
{
	return weapon(m_current_weapon);
}
