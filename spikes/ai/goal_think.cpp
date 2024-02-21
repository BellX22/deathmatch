#include "goal_think.hpp"
#include "goal_explore.hpp"
#include "goal_atomics.hpp"

#include "../the_bot.hpp"

GoalThink::GoalThink(TheBot& owner) : 
	GoalComposite(owner, Type::Think)
{
}

void GoalThink::arbitrate()
{
}

bool GoalThink::not_present(Type type)
{
	if (!m_subgoals.empty()) {
		return m_subgoals.front()->get_type() != type;
	}
	return true;
}

void GoalThink::activate()
{
	if (!m_owner->is_possessed()) {
		arbitrate();
	}
	m_status = Status::Active;
}

Goal::Status GoalThink::update(float dt)
{
	activate_if_inactive();
	Status status = process_subgoals(dt);

	if (status == Status::Completed || status == Status::Failed) {
		if (!m_owner->is_possessed()) {
			m_status = Status::Inactive;
			addgoal_explore();
		}
	}
	return m_status;
}

void GoalThink::addgoal_explore()
{
	if (not_present(Type::Explore)) {
		remove_all_subgoals();
		add_subgoal(std::make_unique<GoalExplore>(*m_owner));
	}
}

void GoalThink::addgoal_seek_to_pos(v2 pos)
{
		remove_all_subgoals();
		add_subgoal(std::make_unique<GoalSeekToPosition>(*m_owner, pos));
}
