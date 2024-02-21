#pragma once
#include "goal.hpp"
#include "../math.hpp"
#include "../path_manager.hpp"

class GoalSeekToPosition : public Goal {
public:
	GoalSeekToPosition(TheBot& owner, v2 destination);

	virtual void        activate() override final;
	virtual Status      update(float dt) override final;
	virtual void        terminate() override final;

private:
	bool is_stuck();

private:
	v2       m_destination;
	float    m_time_to_reach_pos;
	float    m_timer;
};

class GoalTraverseEdge : public Goal
{
public:
	GoalTraverseEdge(TheBot& owner, const PathEdge& edge, bool last);

	virtual void        activate() override final;
	virtual Status      update(float dt) override final;
	virtual void        terminate() override final;

private:
	bool is_stuck();

private:
	PathEdge m_edge;
	float    m_timer;
	bool     m_last_edge;
	float    m_esitmated_time;
};
