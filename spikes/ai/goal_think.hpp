#pragma once
#include "goal.hpp"
#include <corelib/math/math.hpp>

///////////////////////////////////////////////////////////////////////////////
// Top Most Goal
///////////////////////////////////////////////////////////////////////////////
class GoalThink : public GoalComposite
{
public:
	GoalThink(TheBot& owner);

	void                arbitrate();
	bool                not_present(Type type);
	virtual void        activate() override final;
	virtual Status      update(float dt) override final;
	virtual void        terminate() override final {}

	void addgoal_explore();
	void addgoal_seek_to_pos(v2 pos);
};
