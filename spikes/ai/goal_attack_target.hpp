#pragma once
#include "goal.hpp"

#include "../math.hpp"

class GoalAttackTarget : public GoalComposite
{
public:
	GoalAttackTarget(TheBot& owner) : GoalComposite(owner, Type::AttackTarget) {}

	virtual void        activate() override final;
	virtual Status      update(float dt) override final;
	virtual void        terminate() override final { m_status == Status::Completed; }

	virtual bool handle_msg(const Message& l_msg) override final {}
};