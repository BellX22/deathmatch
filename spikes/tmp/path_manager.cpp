#include "path_manager.hpp"
#include "graph_helpers.hpp"

#include <cassert>

PathPlaner::PathPlaner(const NavGraph* navgraph)
{
	set_navgraph(navgraph);
}

bool PathPlaner::request_path_to_pos(v2 start, v2 end)
{
	m_destination = end;
	// if a bot can just walk to we do not need a path planing;
	// therefore we return true;

	// get closest node to the bot's position
	int src_node = get_closest_node_from_pos(*m_navgraph, start);
	int dst_node = get_closest_node_from_pos(*m_navgraph, end);
	m_search_algo = std::make_unique<GraphSearchAStarTimeSliced<NavGraph, HeuristicEuclid>>(*m_navgraph, src_node, dst_node);
	return true;
}

std::vector<PathEdge> PathPlaner::get_path()
{
	assert(m_search_algo);
	std::vector<PathEdge> path = m_search_algo->get_path_as_path_edges();

	// push the first edge from bot to the first node of path
	//path.insert(std::begin(path), 1, PathEdge(botpos, firstnodepos));
	return path;
}

SearchResult PathPlaner::cycle_once()
{
	if (m_result == SearchResult::TargetFound) return m_result;
	assert(m_search_algo);
	auto result = m_search_algo->cycle_once();
	m_result = result;
	switch (result) {
	case SearchResult::TargetFound:
		break;
	case SearchResult::TargetNotFound:
		break;
	case SearchResult::SearchIncomplete:
		break;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

void PathManager::update_searches()
{
	for (auto& search : m_search_requests) {
		SearchResult result = SearchResult::SearchIncomplete;
		while (result == SearchResult::SearchIncomplete ||
			   result == SearchResult::TargetNotFound) {
			result = search.second->cycle_once();
		}
	}

	// remove all searches wich are handled
	for (auto it = std::begin(m_search_requests); it != std::end(m_search_requests);)
	{
		if (it->second->is_marked_for_removal()) {
			it = m_search_requests.erase(it);
		}
		else
		{
			it++;
		}
	}
}

int PathManager::create_new_search()
{
	assert(m_navgraph);
	m_search_requests.insert({ m_next_valid_search_id, std::make_unique<PathPlaner>(m_navgraph) });
    return m_next_valid_search_id++;
}

void PathManager::remove_search(int id)
{
	m_search_requests.erase(id);
}

PathPlaner* PathManager::get_search(int id)
{
    return m_search_requests.at(id).get();
}

int PathManager::get_num_active_searches() const
{
    return m_search_requests.size();
}
