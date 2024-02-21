#include "goal_follow_path.hpp"
#include "goal_atomics.hpp"

GoalFollowPath::GoalFollowPath(TheBot& owner, std::list<PathEdge>& path) :
	GoalComposite(owner, Type::FollowPath)
	, m_path(path)
{
}

void GoalFollowPath::activate()
{
	bool last = false;
	m_status = Status::Active;
	PathEdge edge = m_path.front();
	m_path.pop_front();
	if (m_path.empty()) last = true;
	add_subgoal(std::make_unique<GoalTraverseEdge>(*m_owner, edge, last));
}

Goal::Status GoalFollowPath::update(float dt)
{
	activate_if_inactive();
	m_status = process_subgoals(dt);
	if (m_status == Status::Completed && !m_path.empty()) {
		activate();
	}
	return m_status;
}

void GoalFollowPath::terminate()
{
}
