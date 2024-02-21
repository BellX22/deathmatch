#include <stack>
#include <list>

template<class graphType, class heuristic>
GraphSearchAStarTimeSliced<graphType, heuristic>::GraphSearchAStarTimeSliced
(const graphType& graph, int source, int target) : 
	GraphSearchTimeSliced<Edge>(GraphSearchTimeSliced::AStar),
m_graph(graph),
m_shortestPathTree(graph.get_num_nodes()),
m_searchFrontier(graph.get_num_nodes()),
m_Gcosts(graph.get_num_nodes(), 0.f),
m_Fcosts(graph.get_num_nodes(), 0.f),
m_source(source),
m_target(target),
m_priorityQueue([](NodeAndCost elem1, NodeAndCost elem2) {return elem1.second > elem2.second; })
{
	m_priorityQueue.push(std::make_pair(m_source, 0.f));
}

template<class graphType, class heuristic>
inline SearchResult GraphSearchAStarTimeSliced<graphType, heuristic>::cycle_once()
{
	if (m_priorityQueue.empty())
	{
		return SearchResult::TargetNotFound;
	}

	int nextClosestNode = m_priorityQueue.top().first;
	m_priorityQueue.pop();

	m_shortestPathTree[nextClosestNode] = m_searchFrontier[nextClosestNode];

	if (nextClosestNode == m_target)
	{
		return SearchResult::TargetFound;
	}

	for (auto edgeIt = m_graph.m_edges[nextClosestNode].cbegin(); edgeIt != m_graph.m_edges[nextClosestNode].cend(); edgeIt++)
	{
		float hCost = heuristic::calculate(m_graph, m_target, edgeIt->get_to());
		float gCost = m_Gcosts[nextClosestNode] + edgeIt->get_cost();

		if (m_searchFrontier[edgeIt->get_to()] == nullptr)
		{
			m_Fcosts[edgeIt->get_to()] = gCost + hCost;
			m_Gcosts[edgeIt->get_to()] = gCost;

			m_priorityQueue.push(std::make_pair(edgeIt->get_to(), gCost + hCost));

			m_searchFrontier[edgeIt->get_to()] = &(*edgeIt);
		}
		else if ((gCost < m_Gcosts[edgeIt->get_to()]) && (m_shortestPathTree[edgeIt->get_to()] == nullptr))
		{
			m_Fcosts[edgeIt->get_to()] = gCost + hCost;
			m_Gcosts[edgeIt->get_to()] = gCost;

			m_priorityQueue.push(std::make_pair(edgeIt->get_to(), gCost + hCost));

			m_searchFrontier[edgeIt->get_to()] = &(*edgeIt);
		}
	}

	return SearchResult::SearchIncomplete;
}

template<class graphType, class heuristic>
inline std::vector<int> GraphSearchAStarTimeSliced<graphType, heuristic>::get_path_to_target() const
{
	std::list<int> path;

	if (m_target < 0) return std::vector<int>(path.begin(), path.end());

	int node = m_target;
	path.push_back(node);
	while ((node != m_source) && (m_shortestPathTree[node] != nullptr))
	{
		node = m_shortestPathTree[node]->get_from();
		path.push_front(node);
	}

	return std::vector<int>(path.begin(), path.end());
}

template<class graphType, class heuristic>
inline std::vector<PathEdge> GraphSearchAStarTimeSliced<graphType, heuristic>::get_path_as_path_edges() const
{
	std::list<PathEdge> path;

	if (m_target < 0) return std::vector<PathEdge>();

	int node = m_target;

	while ((node != m_source) && (m_shortestPathTree[node] != nullptr))
	{
		path.push_front(PathEdge(
			m_graph.get_node(m_shortestPathTree[node]->get_from()).get_pos(),
			m_graph.get_node(m_shortestPathTree[node]->get_to()).get_pos(),
			m_shortestPathTree[node]->get_flags(),
			m_shortestPathTree[node]->get_id_of_intersecting_entity()));

		node = m_shortestPathTree[node]->get_from();
	}

	return std::vector<PathEdge>(path.begin(), path.end());
}

template<class graphType, class terminationCondition>
	GraphSearchDijkstraTimeSliced<graphType, terminationCondition>::GraphSearchDijkstraTimeSliced
	(const graphType& graph, int source, int target) : 
		GraphSearchTimeSliced<Edge>(GraphSearchTimeSliced::Dijkstra),
		m_graph(graph),
		m_shortestPathTree(graph.get_num_nodes(), nullptr),
		m_searchFrontier(graph.get_num_nodes(), nullptr),
		m_costToThisNode(graph.get_num_nodes(), std::numeric_limits<float>::infinity()),
		m_source(source),
		m_target(target),
		m_priorityQueue([](NodeAndCost elem1, NodeAndCost elem2) {return elem1.second > elem2.second; })
	{
		m_priorityQueue.push(std::make_pair(m_source, 0.f));
		m_costToThisNode[m_source] = 0.f;
	}

template<class graphType, class terminationCondition>
inline SearchResult GraphSearchDijkstraTimeSliced<graphType, terminationCondition>::cycle_once()
{
	if (m_priorityQueue.empty())
	{
		return SearchResult::TargetNotFound;
	}

	int nextClosestNode = m_priorityQueue.top().first;
	m_priorityQueue.pop();

	m_shortestPathTree[nextClosestNode] = m_searchFrontier[nextClosestNode];

	if (terminationCondition::calculate(m_graph, m_target, nextClosestNode))
	{
		m_target = nextClosestNode;
		return SearchResult::TargetFound;
	}

	for (auto edgeIt = m_graph.m_edges[nextClosestNode].cbegin(); edgeIt != m_graph.m_edges[nextClosestNode].cend(); edgeIt++)
	{
		float newCost = m_costToThisNode[nextClosestNode] + edgeIt->get_cost();


		if (m_searchFrontier[edgeIt->get_to()] == nullptr)
		{
			m_costToThisNode[edgeIt->get_to()] = newCost;

			m_priorityQueue.push(std::make_pair(edgeIt->get_to(), newCost));

			m_searchFrontier[edgeIt->get_to()] = &(*edgeIt);
		}
		else if ((newCost < m_costToThisNode[edgeIt->get_to()]) && (m_shortestPathTree[edgeIt->get_to()] == nullptr))
		{
			m_costToThisNode[edgeIt->get_to()] = newCost;

			m_priorityQueue.push(std::make_pair(edgeIt->get_to(), newCost));

			m_searchFrontier[edgeIt->get_to()] = &(*edgeIt);
		}
	}

	return SearchResult::SearchIncomplete;
}

template<class graphType, class terminationCondition>
inline std::vector<int> GraphSearchDijkstraTimeSliced<graphType, terminationCondition>::get_path_to_target() const
{
	std::list<int> path;

	if (m_target < 0) return std::vector<int>(0);

	int node = m_target;

	path.push_back(node);

	while ((node != m_source) && (m_shortestPathTree[node] != nullptr))
	{
		node = m_shortestPathTree[node]->get_from();

		path.push_front(node);
	}

	return std::vector<int>(path.begin(), path.end());
}

template<class graphType, class terminationCondition>
inline std::vector<PathEdge> GraphSearchDijkstraTimeSliced<graphType, terminationCondition>::get_path_as_path_edges() const
{
	std::list<PathEdge> path;

	if (m_target < 0) return std::vector<PathEdge>();

	int node = m_target;

	while ((node != m_source) && (m_shortestPathTree[node] != nullptr))
	{
		path.push_front(PathEdge(
			m_graph.get_node(m_shortestPathTree[node]->get_from()).get_pos(),
			m_graph.get_node(m_shortestPathTree[node]->get_to()).get_pos(),
			m_shortestPathTree[node]->get_flags(),
			m_shortestPathTree[node]->get_id_of_intersecting_entity())
		);

		node = m_shortestPathTree[node]->get_from();
	}

	return std::vector<PathEdge>(path.begin(), path.end());
}

template <class graphType>
bool GraphSearchDFS<graphType>::search()
{
	//create a std stack of edges
	std::stack<const Edge*> stack;

	//create a dummy edge and put on the stack
	Edge dummy(m_source, m_source, 0);

	stack.push(&dummy);

	//while there are edges in the stack keep searching
	while (!stack.empty())
	{
		//grab the next edge
		const Edge* next = stack.top();

		//remove the edge from the stack
		stack.pop();

		//make a note of the parent of the node this edge points to
		m_route[next->get_to()] = next->from();

		//put it on the tree. (making sure the dummy edge is not placed on the tree)
		if (next != &dummy)
		{
			m_spanningTree.push_back(next);
		}

		//and mark it visited
		m_visited[next->get_to()] = Visited;

		//if the target has been found the method can return success
		if (next->get_to() == m_target)
		{
			return true;
		}

		//push the edges leading from the node this edge points to onto
		//the stack (provided the edge does not point to a previously 
		//visited node)
		graphType::const_iterator_edge it = m_graph.cbegin_edge(next->get_to());

		for (; it != m_graph.cend_edge(next->get_to()); i++)
		{
			if (m_visited[it->get_to()] == Unvisited)
			{
				stack.push(it.getPtr());
			}
		}
	}

	//no path to target
	return false;
}

template <class graphType>
std::vector<int> GraphSearchDFS<graphType>::getPathToTarget() const
{
	std::list<int> path;

	if (!m_found || m_target < 0) return path;

	int node = m_target;

	path.push_front(node);

	while (node != m_source)
	{
		node = m_Route[node];

		path.push_front(node);
	}

	return path;
}

template<class graphType>
inline std::vector<int> GraphSearchBFS<graphType>::getPathToTarget() const
{
	std::vector<int> path;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!m_found || m_target < 0) return path;

	int node = m_target;

	path.push_front(node);

	while (node != m_source)
	{
		node = m_Route[node];

		path.push_front(node);
	}

	return path;
}

template<class graphType>
inline bool GraphSearchBFS<graphType>::search()
{
	//create a std queue of edges
	std::queue<const Edge*> queue;

	const Edge dummy(m_source, m_source, 0);

	//create a dummy edge and put on the queue
	queue.push(&dummy);

	//mark the source node as visited
	m_visited[m_source] = Visited;

	//while there are edges in the queue keep searching
	while (!queue.empty())
	{
		//grab the next edge
		const Edge* next = queue.front();

		queue.pop();

		//mark the parent of this node
		m_route[next->get_to()] = next->get_from();

		//put it on the tree. (making sure the dummy edge is not placed on the tree)
		if (next != &dummy)
		{
			m_spanningTree.push_back(next);
		}

		//exit if the target has been found
		if (next->get_to() == m_target)
		{
			return true;
		}

		graphType::const_iterator_edge it = m_graph.cbegin_edge(next->get_to());

		for (; it != m_graph.cend_edge(next->get_to()); i++)
		{
			if (m_visited[it->get_to()] == Unvisited)
			{
				queue.push(it.getPtr());
				m_visited[it->get_to()] = Visited;
			}
		}
	}

	//no path to target
	return false;
}
