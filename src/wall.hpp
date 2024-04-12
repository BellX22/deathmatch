#pragma once
#include "utils.hpp"
#include <iostream>
#include <memory>

struct Wall {
	using Ptr = std::unique_ptr<Wall>;

	Wall() = default;
	Wall(v2 start, v2 end);
	auto start() const -> v2;
	auto end() const -> v2;
	auto normal() const -> v2;
	auto direction() const -> v2;
	auto center() const -> v2;
	auto blocks_movement() const -> bool;
	auto blocks_projectiles() const -> bool;

	auto deserialize(std::istream &in) -> void;
	auto serialize(std::ostream &out) -> void;
private:
	v2 m_start{0, 0};
	v2 m_end{0, 0};
	bool m_blocks_movement{true};
	bool m_blocks_projectiles{true};
};
