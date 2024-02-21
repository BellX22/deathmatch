#pragma once

#include <unordered_map>
#include "graph.hpp"
#include "graph_algorithms.hpp"

class PathManager;

class PathPlaner {
public:
	using Ptr = std::unique_ptr<PathPlaner>;

	PathPlaner(const NavGraph* navgraph = nullptr);

	void                  set_navgraph(const NavGraph* graph) { m_navgraph = graph; }
	bool                  request_path_to_pos(v2 start, v2 end);
	std::vector<PathEdge> get_path();
	SearchResult          cycle_once();
	bool                  is_marked_for_removal() { return m_is_marked_for_removal; }
	void                  mark_for_removal() { m_is_marked_for_removal = true; }
	SearchResult          get_result() const { return m_result; }

private:
	const NavGraph* m_navgraph;
	v2           m_destination;
	bool         m_is_marked_for_removal = false;
	SearchResult m_result = SearchResult::SearchIncomplete;
	std::unique_ptr<GraphSearchAStarTimeSliced<NavGraph, HeuristicEuclid>> m_search_algo;
};

class PathManager {
public:
	void        update_searches();

	void        set_navgraph(const NavGraph* graph) { m_navgraph = graph; }

	int         create_new_search();
	void        remove_search(int id);

	PathPlaner* get_search(int id);
	
	int         get_num_active_searches() const;
private:
	std::unordered_map<int, PathPlaner::Ptr> m_search_requests;
	const NavGraph*                          m_navgraph = nullptr;
	int                                      m_next_valid_search_id = 0;
};
