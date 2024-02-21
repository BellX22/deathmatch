#pragma once

#include "graph.hpp"
#include <corelib/math/math.hpp>

#include <queue>
#include <functional>

///////////////////////////////////////////////////////////////////////////////
// A Star Heuristics
///////////////////////////////////////////////////////////////////////////////
struct HeuristicEuclid {
	//calculate the straight line distance from node nd1 to node nd2
	template <class graphType>
	static float calculate(const graphType& G, int nd1, int nd2) {
		return length(G.get_node(nd1).get_pos() - G.get_node(nd2).get_pos());
	}
};
struct HeuristicNoisyEuclidian {
	//calculate the straight line distance from node nd1 to node nd2
	template <class graphType>
	static float calculate(const graphType& G, int nd1, int nd2) {
		return length(G.get_node(nd1).get_pos() - G.get_node(nd2).get_pos());
	}
};

//-----------------------------------------------------------------------------
//you can use this class to turn the A* algorithm into Dijkstra's search.
//this is because Dijkstra's is equivalent to an A* search using a heuristic
//value that is always equal to zero.
//-----------------------------------------------------------------------------
struct HeuristicDijkstra {
	template <class graphType>
	static float calculate(const graphType& G, int nd1, int nd2) {
		return 0;
	}
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// A Helper class for calculation in graphs
///////////////////////////////////////////////////////////////////////////////
class PathEdge {
public:
	PathEdge(v2 src, v2 dst, int behaviour, int door_id = 0):
		m_source{ src }, m_destination{ dst }, m_behaviour{ behaviour }, m_door_id{ door_id }{}

	v2   get_destination() const { return m_destination; }
	void set_destination(v2 dst) { m_destination = dst; }

	v2   get_source() const { return m_source; }
	void set_source(v2 src) { m_source = src; }

	int  get_behaviour() const { return m_behaviour; }
	int  get_door_id() const { return m_door_id; }

private:
	v2 m_source, m_destination;
	int m_behaviour;
	int m_door_id;
};
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Algorithms enums
///////////////////////////////////////////////////////////////////////////////
enum class SearchResult
{
	TargetFound,
	TargetNotFound,
	SearchIncomplete
};

///////////////////////////////////////////////////////////////////////////////
// General Interface
///////////////////////////////////////////////////////////////////////////////

template<class edgeType>
class GraphSearchTimeSliced {
public:
	enum SearchType {
		AStar,
		Dijkstra
	};

public:
	GraphSearchTimeSliced(SearchType type) : m_search_type(type) {}
	virtual ~GraphSearchTimeSliced() {}

	virtual SearchResult                 cycle_once() = 0;
	virtual std::vector<const edgeType*> get_shortest_path_tree() const = 0;
	virtual float                        get_cost_to_target() const = 0;
	virtual std::vector<int>             get_path_to_target() const = 0;
	//virtual std::vector<PathEdge>        get_path_as_path_edges() const = 0;

	SearchType get_search_type() const { return m_search_type; }

private:
	SearchType m_search_type;
};

///////////////////////////////////////////////////////////////////////////////
// A Star
///////////////////////////////////////////////////////////////////////////////

template<class graphType, class heuristic>
class GraphSearchAStarTimeSliced : public GraphSearchTimeSliced<typename graphType::EdgeType>
{
	typedef typename graphType::EdgeType Edge;
	typedef typename graphType::NodeType Node;
	typedef std::pair<int, float> NodeAndCost;

public:
	GraphSearchAStarTimeSliced(const graphType& graph, int source, int target);

	virtual SearchResult             cycle_once() override final;
	virtual std::vector<const Edge*> get_shortest_path_tree() const override final { return m_shortestPathTree; }
	virtual float                    get_cost_to_target() const override final { return m_Gcosts[m_target]; }
	virtual std::vector<int>         get_path_to_target() const override final;

	std::vector<PathEdge>              get_path_as_path_edges() const;


private:
	const graphType&   m_graph;
	//indexed into my node. Contains the 'real' accumulative cost to that node
	std::vector<float> m_Gcosts;
	//indexed into by node. Contains the cost from adding m_GCosts[n] to
	//the heuristic cost from n to the target node. This is the vector the
	//iPQ indexes into.
	std::vector<float> m_Fcosts;
	std::vector<const Edge*> m_shortestPathTree;
	std::vector<const Edge*> m_searchFrontier;

	int m_source;
	int m_target;

	std::priority_queue<NodeAndCost, std::vector<NodeAndCost>, std::function<bool(NodeAndCost, NodeAndCost)>> m_priorityQueue;
};

///////////////////////////////////////////////////////////////////////////////
// Dijkstra
///////////////////////////////////////////////////////////////////////////////

template<class graphType, class terminationCondition>
class GraphSearchDijkstraTimeSliced : public GraphSearchTimeSliced<typename graphType::EdgeType>
{
	typedef typename graphType::EdgeType Edge;
	typedef typename graphType::NodeType Node;
	typedef std::pair<int, float> NodeAndCost;

public:
	GraphSearchDijkstraTimeSliced(const graphType& graph, int source, int target);

	virtual SearchResult             cycle_once() override final;
	virtual std::vector<const Edge*> get_shortest_path_tree() const override final { return m_shortestPathTree; }
	virtual float                    get_cost_to_target() const override final { return m_costToThisNode[m_target]; }
	virtual std::vector<int>         get_path_to_target() const override final;

	std::vector<PathEdge>              get_path_as_path_edges() const;

private:
	const graphType&         m_graph;
	std::vector<float>       m_costToThisNode;
	std::vector<const Edge*> m_shortestPathTree;
	std::vector<const Edge*> m_searchFrontier;

	int m_source;
	int m_target;

	std::priority_queue<NodeAndCost, std::vector<NodeAndCost>, std::function<bool(NodeAndCost, NodeAndCost)>> m_priorityQueue;
};

///////////////////////////////////////////////////////////////////////////////
// Depth First Search
///////////////////////////////////////////////////////////////////////////////
template<class graphType>
class GraphSearchDFS
{
	enum
	{
		Visited,
		Unvisited,
		NoParentAssigned
	};

	typedef typename graphType::NodeType Node;
	typedef typename graphType::EdgeType Edge;

public:
	GraphSearchDFS(const graphType& graph, int source, int target = -1) :
		m_graph(graph),
		m_source(source),
		m_target(target),
		m_visited(m_graph.getNumNodes(), Unvisited),
		m_route(m_graph.getNumNodes(), NoParentAssigned),
		m_found(false) {
		m_found = search();
	}

	std::vector<const Edge*> getSearchTree() const { return m_spanningTree; }
	bool                     hasFound() const { return m_found; }
	std::vector<int>         getPathToTarget() const;

private:
	bool search();

private:
	graphType&               m_graph;
	std::vector<int>         m_visited;
	std::vector<int>         m_route;
	std::vector<const Edge*> m_spanningTree;
	int                      m_source, m_target;
	bool                     m_found;
};

///////////////////////////////////////////////////////////////////////////////
// Breadth First Search
///////////////////////////////////////////////////////////////////////////////
template<class graphType>
class GraphSearchBFS
{
	enum
	{
		Visited,
		Unvisited,
		NoParentAssigned
	};

	typedef typename graphType::NodeType Node;
	typedef typename graphType::EdgeType Edge;

public:
	GraphSearchBFS(const graphType& graph, int source, int target = -1) :
		m_graph(graph),
		m_source(source),
		m_target(target),
		m_visited(m_graph.getNumNodes(), Unvisited),
		m_route(m_graph.getNumNodes(), NoParentAssigned),
		m_found(false) {
		m_found = search();
	}

	std::vector<const Edge*> getSearchTree() const { return m_spanningTree; }
	bool                     hasFound() const { return m_found; }
	std::vector<int>         getPathToTarget() const;

private:
	bool search();

private:
	graphType&               m_graph;
	std::vector<int>         m_visited;
	std::vector<int>         m_route;
	std::vector<const Edge*> m_spanningTree;
	int                      m_source, m_target;
	bool                     m_found;
};

#include "graph_algorithms.inl"
