#include "goal_atomics.hpp"

#include "../the_bot.hpp"

static float LONG_WAIT = 15.f;// sec

GoalSeekToPosition::GoalSeekToPosition(TheBot& owner, v2 destination) : 
	Goal(owner, Type::SeekToPosition)
	, m_destination{destination}
	, m_time_to_reach_pos{}
	, m_timer{}
{
}

void GoalSeekToPosition::activate()
{
	m_status = Status::Active;
	m_timer = 0.f;
	m_time_to_reach_pos = LONG_WAIT;
	m_time_to_reach_pos += 1.f;
	m_owner->get_steering().set_target(m_destination);
	m_owner->get_steering().seek_on();
}

Goal::Status GoalSeekToPosition::update(float dt)
{
	activate_if_inactive();
	if (is_stuck()) {
		m_status = Status::Failed;
	}
	else {
		if (m_owner->is_at_position(m_destination)) {
			m_status = Status::Completed;
		}
	}
	return m_status;
}

void GoalSeekToPosition::terminate()
{
	m_owner->get_steering().seek_off();
	m_status = Status::Completed;
}

bool GoalSeekToPosition::is_stuck()
{
	if (m_timer > m_time_to_reach_pos) {
		return true;
	}
	return false;
}

GoalTraverseEdge::GoalTraverseEdge(TheBot& owner, const PathEdge& edge, bool last) :
	Goal(owner, Type::TraverseEdge)
	, m_edge(edge)
	, m_timer{ 0.f }
	, m_last_edge{ last }
	, m_esitmated_time{10.f}
{
}

void GoalTraverseEdge::activate()
{
	m_status = Status::Active;
	//m_esitmated_time = m_owner->get_time_to_reach_pos(m_edge.get_destination());
	m_owner->get_steering().set_target(m_edge.get_destination());

	if (m_last_edge) {
		m_owner->get_steering().arrive_on();
	}
	else
	{
		m_owner->get_steering().seek_on();
	}
}

Goal::Status GoalTraverseEdge::update(float dt)
{
	m_timer += dt;
	activate_if_inactive();
	if (is_stuck()) {
		m_status = Status::Failed;
	}
	else {
		if (m_owner->is_at_position(m_edge.get_destination())) {
			m_status = Status::Completed;
		}
	}
	return m_status;
}

void GoalTraverseEdge::terminate()
{
	m_owner->get_steering().seek_off();
	m_owner->get_steering().arrive_off();
}

bool GoalTraverseEdge::is_stuck()
{
	return false;
}
