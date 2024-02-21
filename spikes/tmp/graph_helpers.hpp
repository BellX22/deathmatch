#pragma once
#include "graph.hpp"
#include <corelib/math/math.hpp>

inline int get_closest_node_from_pos(const NavGraph& graph, v2 pos) {
	float distance = std::numeric_limits<float>::max();
	int node_index = -1;
	for (auto& node : graph.m_nodes) {
		float cur_distance = length(pos - node.get_pos());
		if (cur_distance < distance) {
			distance = cur_distance;
			node_index = node.get_index();
		}
	}
	return node_index;
}
