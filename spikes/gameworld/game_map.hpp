#pragma once
#include "wall.hpp"
#include "door.hpp"

class GameMap
{

public:
    auto update(float dt_sec) -> void;

    auto add_wall(v2 start, v2 end) -> index_t;
    auto add_door(index_t wall1, index_t wall2) -> void;

	auto& get_walls() { return m_walls; }
	auto const& get_walls() const { return m_walls; }
    auto& get_doors() { return m_doors; }
    auto const& get_doors() const { return m_doors; }
    auto& get_spawnpoints() { return m_spawnpoints; }
    auto const& get_spawnpoints() const { return m_spawnpoints; }

private:
    auto calc_map_bounds() -> void;
private:
    std::vector<Wall> m_walls;
    std::vector<Door> m_doors;
    std::vector<v2>   m_spawnpoints;
    v2                m_min{0, 0};
    v2                m_max{0, 0};
};

#if 0
v2 get_random_node_location()
{
	int nodecount = m_navgraph.get_num_active_nodes();
	int destnode = math::rand_float() * nodecount;
	v2 pos = m_navgraph.get_node(destnode).get_pos();
	return pos;
}

struct NodeIndex {
	int index;
	int i, j;
};

static int get_node_index(const std::vector<NodeIndex>& indices, int x, int y) {
	for (auto index : indices) {
		if (index.i == y && index.j == x) {
			return index.index;
		}
	}
	return -1;
}

static void add_edges(NavGraph* graph, const std::vector<std::vector<int>>& walls, const std::vector<NodeIndex>& indices, int x, int y) {
	int height = walls.size();
	int width = walls[0].size();

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (int to = get_node_index(indices, x + j, y + i); to >= 0) {
				int from = get_node_index(indices, x, y);
				auto node1 = graph->m_nodes[to];
				auto node2 = graph->m_nodes[from];
				auto len = length(node1.get_pos() - node2.get_pos());
				graph->add_edge(NavGraphEdge(from, to, len));
			}
		}
	}
}

void calc_navgraph()
{
	std::vector<NodeIndex> indices;

	m_navgraph.clear();
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			if (get_texture_id_block(j, i) == 0) {
				int nodeindex = m_navgraph.add_node(NavGraphNode({ j + .5f, i + .5f }));
				indices.push_back({ nodeindex, i, j });
			}
		}
	}

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			if (get_texture_id_block(j, i) == 0) {
				add_edges(&m_navgraph, m_walls, indices, j, i);
			}
		}
	}
}
#endif
