#pragma once
#include "goal.hpp"

#include "../math.hpp"

class GoalExplore : public GoalComposite
{
public:
	GoalExplore(TheBot& owner);

	virtual void        activate() override final;
	virtual Status      update(float dt) override final;
	virtual void        terminate() override final;

	virtual bool handle_msg(const Message& l_msg) override final;

private:
	v2 m_current_dest;
	bool m_dest_in_set;
	int m_path_request;
	class PathPlaner* m_path_planer = nullptr;
};
