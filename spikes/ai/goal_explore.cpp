#include "goal_explore.hpp"
#include "goal_atomics.hpp"
#include "goal_follow_path.hpp"

#include "../the_bot.hpp"
#include "../path_manager.hpp"
#include "../game_world.hpp"

GoalExplore::GoalExplore(TheBot& owner) :
	GoalComposite(owner, Type::Explore)
	, m_dest_in_set{ false }
	, m_path_request{ -1 }
{
}

void GoalExplore::activate()
{
	m_status = Status::Active;
	remove_all_subgoals();
	if (!m_dest_in_set) {
		m_current_dest = m_owner->get_world().get_map().get_random_node_location();
		m_dest_in_set = true;
	}

	// request a path 
	auto& pathmanager = m_owner->get_world().get_path_manager();
	m_path_request = pathmanager.create_new_search();
	m_path_planer = pathmanager.get_search(m_path_request);
	m_path_planer->request_path_to_pos(m_owner->get_pos(), m_current_dest);

	// while we are waiting for a path result we push a new Goal
	// actually path requests are instantly so we do not need it
	add_subgoal(std::make_unique<GoalSeekToPosition>(*m_owner, m_current_dest));
}

Goal::Status GoalExplore::update(float dt)
{
	activate_if_inactive();
	m_status = process_subgoals(dt);

	if (m_path_planer) {
		auto result = m_path_planer->get_result();
		if (result == SearchResult::TargetFound) {
			auto pv = m_path_planer->get_path();
			std::list<PathEdge> p = std::list(pv.begin(), pv.end());
			m_path_planer->mark_for_removal();
			m_path_planer = nullptr;
			remove_all_subgoals();
			add_subgoal(std::make_unique<GoalFollowPath>(*m_owner, p));
		}
		else if (result == SearchResult::TargetNotFound) {
			m_status = Status::Failed;
		}
	}

	return m_status;
}

void GoalExplore::terminate()
{
}

bool GoalExplore::handle_msg(const Message& msg)
{
	bool handled = forward_msg_to_front_most_subgoal(msg);
	//if (!handled) {
	//	switch (msg.kind) {
	//	case MessageKind::PathReady:
	//		remove_all_subgoals();
	//		add_subgoal(std::make_unique<GoalFollowPath>(*m_owner, path));
	//		return true;
	//	case MessageKind::NoPathAvailable:
	//		m_status = Status::Failed;
	//		return true;
	//	default:
	//		return false;
	//	}
	//}
	// handled by subgoals
	return true;
}
