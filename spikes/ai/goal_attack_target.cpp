#include "goal_attack_target.hpp"

void GoalAttackTarget::activate()
{
	m_status = Status::Active;
	remove_all_subgoals();
}

Goal::Status GoalAttackTarget::update(float dt)
{
	activate_if_inactive();
	m_status = process_subgoals(dt);
	reactivate_if_failed();
	return m_status;
}
