#pragma once
#include "goal.hpp"

#include "../path_manager.hpp"

class GoalFollowPath : public GoalComposite
{
public:
	GoalFollowPath(TheBot& owner, std::list<PathEdge>& path);

	virtual void        activate() override final;
	virtual Status      update(float dt) override final;
	virtual void        terminate() override final;

	auto&               get_path() { return m_path; }

private:
	std::list<PathEdge> m_path;
};
