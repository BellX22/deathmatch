#pragma once

#include <memory>
#include <cassert>
#include <list>

class TheBot;
struct Message;

class Goal {
public:
	using Ptr = std::unique_ptr<Goal>;
	enum class Type {
		Think,
		AttackTarget,
		Explore,
		FollowPath,
		TraverseEdge,   // atmic
		SeekToPosition, // atmic
	};
	enum class Status {
		Active,
		Inactive,
		Completed,
		Failed
	};

public:
	Goal(TheBot& owner, Type type) : m_owner{ &owner }, m_type{ type }, m_status{ Status::Inactive }{}
	virtual ~Goal() {}

	virtual void     activate() = 0;
	virtual Status   update(float dt) = 0;
	virtual void     terminate() = 0;

	virtual bool     handle_msg(const Message& msg) { return false; }
	virtual void     add_subgoal(Ptr goal) { assert(false && "can not add goals to atomics"); }

	bool             is_complete()  const { return m_status == Status::Completed; }
	bool             is_active()    const { return m_status == Status::Active; }
	bool             is_inactive()  const { return m_status == Status::Inactive; }
	bool             has_failed()   const { return m_status == Status::Failed; }

	Type             get_type() const { return m_type; }
	Status           get_status() const { return m_status; }

	virtual std::list<Goal*> get_subgoals() { /*assert(false && "can not retrieve subgoals from atomic");*/ return std::list<Goal*>(); }

	const char* type_to_string() const {
		switch (m_type) {
		case Goal::Type::Think:
			return "Think";
		case Goal::Type::AttackTarget:
			return "AttackTarget";
		case Goal::Type::Explore:
			return "Explore";
		case Goal::Type::FollowPath:
			return "FollowPath";
		case Goal::Type::TraverseEdge:
			return "TraverseEdge";
		case Goal::Type::SeekToPosition:
			return "SeekToPosition";
			break;
		default:
			return "Unknown";
		}
	}

	const char* status_to_string() const {
		switch (m_status) {
		case Status::Active:
			return "Active";
		case Status::Inactive:
			return "Inactive";
		case Status::Completed:
			return "Completed";
		case Status::Failed:
			return "Failed";
		default:
			return "Unknown";
		}
	}

protected:
	void activate_if_inactive() {
		if (is_inactive()) {
			activate();
		}
	}

	void reactivate_if_failed() {
		if (has_failed()) {
			m_status = Status::Inactive;
		}
	}

protected:
	TheBot*    m_owner;
	Type       m_type;
	Status     m_status;
};

class GoalComposite : public Goal {
public:
	GoalComposite(TheBot& owner, Type type) : Goal(owner, type) {}
	virtual ~GoalComposite() { remove_all_subgoals(); }

	virtual void        activate() = 0;
	virtual Status      update(float dt) = 0;
	virtual void        terminate() = 0;

	bool                handle_msg(const Message& msg) override { return forward_msg_to_front_most_subgoal(msg); }

	void add_subgoal(Ptr goal) override final{
		m_subgoals.push_front(std::move(goal));
	}

	virtual void remove_all_subgoals() final{
		for (auto& g : m_subgoals) {
			g->terminate();
		}
		m_subgoals.clear();
	}

	std::list<Goal*> get_subgoals() override final {
		std::list<Goal*> goals;
		for(auto& g : m_subgoals){
			goals.push_front(g.get());
		}
		return goals;
	}

protected:
	Status  process_subgoals(float dt) {
		while (!m_subgoals.empty() && (m_subgoals.front()->is_complete() || m_subgoals.front()->has_failed())) {
			m_subgoals.front()->terminate();
			m_subgoals.pop_front();
		}

		if (!m_subgoals.empty()) {
			Status status = m_subgoals.front()->update(dt);

			if (status == Status::Completed && m_subgoals.size() > 1) {
				return Status::Active;
			}

			return status;
		}
		else {
			return Status::Completed;
		}
	}

	bool forward_msg_to_front_most_subgoal(const Message& msg) {
		if (!m_subgoals.empty()) {
			return m_subgoals.front()->handle_msg(msg);
		}
		return false;
	}

protected:
	std::list<Ptr> m_subgoals;
};
