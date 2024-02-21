#pragma once

#include <corelib/math/math.hpp>
#include <vector>

class GraphNode {
public:
	GraphNode() : m_index(-1) {}
	GraphNode(int index) : m_index(index) {}
	virtual ~GraphNode() {}

	int get_index() const { return m_index; }
	void set_index(int index) { m_index = index; }
protected:
	int m_index;
};

template<class T = void*>
class NavGraphNode : public GraphNode
{
public:
	NavGraphNode() : GraphNode(), m_position{}, m_data{}{}
	NavGraphNode(v2 pos) : GraphNode(), m_position{pos}, m_data{}{}
	NavGraphNode(int index, v2 pos) : GraphNode(index), m_position{ pos }, m_data{}{}
	virtual ~NavGraphNode() {}

	auto pos() const -> v2 { return m_position; }
	auto set_pos(const v2& pos) -> void { m_position = pos; }

	auto data() const -> T const& { return m_data; }
	auto set_data(T const& data) { m_data = data; }
protected:
	v2 m_position;
	T  m_data;
};

class GraphEdge {
public:
	GraphEdge() : m_from(-1), m_to(-1), m_cost(1.f) {}
	GraphEdge(int from, int to) : m_from(from), m_to(to), m_cost(1.f) {}
	GraphEdge(int from, int to, float cost) : m_from(from), m_to(to), m_cost(cost) {}

	virtual ~GraphEdge() {}

	int   get_from() const { return m_from; }
	void  set_from(int index) { m_from = index; }

	int   get_to() const { return m_to; }
	void  set_to(int index) { m_to = index; }

	float get_cost() const { return m_cost; }
	void  set_cost(float cost) { m_cost = cost; }

	bool operator==(const GraphEdge& rhs) { return rhs.m_from == m_from && rhs.m_to == m_to && rhs.m_cost == m_cost; }
	bool operator!=(const GraphEdge& rhs) { return !(*this == rhs); }

protected:
	int m_from, m_to;
	float m_cost;
};

class NavGraphEdge : public GraphEdge
{
public:
	enum
	{
		Normal,
		Swim,
		Crawl,
		//....
	};
public:
	NavGraphEdge(int from, int to, float cost, int flags = 0, int id = -1) : GraphEdge(from, to, cost), m_flags(flags), m_idOfIntersectingEntity(id) {}

	int  get_flags() const { return m_flags; }
	void set_flags(int flags) { m_flags = flags; }

	int  get_id_of_intersecting_entity() const { return m_idOfIntersectingEntity; }
	void set_id_of_intersecting_entity(int id) { m_idOfIntersectingEntity = id; }

protected:
	int m_flags;
	int m_idOfIntersectingEntity;
};

template<class nodeType, class edgeType>
class SparseGraph
{
public:
	typedef nodeType NodeType;
	typedef edgeType EdgeType;
public:
	SparseGraph(bool directedGraph = false);

	const nodeType& get_node(int index) const;
	nodeType&       get_node(int index);

	const edgeType& get_edge(int from, int to) const;
	edgeType&       get_edge(int from, int to);

	int             get_next_valid_node_index() const;

	int             add_node(nodeType node);
	void            remove_node(int node);

	void            add_edge(edgeType edge);
	void            remove_edge(int from, int to);

	void            set_edge_cost(int from, int to, float cost);

	int             get_num_nodes() const;
	int             get_num_active_nodes() const;
	int             get_num_edges() const;

	bool            is_digraph() const;
	bool            is_empty() const;

	bool            is_node_present(int index) const;
	bool            is_edge_present(int from, int to) const;

	void            clear();
	void            remove_edges();

	std::vector<nodeType>              m_nodes;
	std::vector<std::vector<edgeType>> m_edges;

private:
	bool            is_edge_unique(int from, int to) const;
	void            cull_invalid_edges();

private:
	bool                               m_is_digraph;
	int                                m_next_valid_node_index;
};

#include "graph.inl"

using NavGraph = SparseGraph<NavGraphNode<>, NavGraphEdge>;
