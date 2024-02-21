
template<class nodeType, class edgeType>
SparseGraph<nodeType, edgeType>::SparseGraph(bool directed) : m_next_valid_node_index(0), m_is_digraph(directed)
{

}

template<class nodeType, class edgeType>
const nodeType& SparseGraph<nodeType, edgeType>::get_node(int index) const
{
	assert((index < (int)m_nodes.size()) && (index >= 0) && "<SparseGraph::node>: invalid index");

	return m_nodes[index];
}

template<class nodeType, class edgeType>
nodeType& SparseGraph<nodeType, edgeType>::get_node(int index)
{
	assert((index < (int)m_nodes.size()) && (index >= 0) && "<SparseGraph::node>: invalid index");

	return m_nodes[index];
}

template<class nodeType, class edgeType>
const edgeType& SparseGraph<nodeType, edgeType>::get_edge(int from, int to) const
{
	assert((from < m_nodes.size()) && (from >= 0) && (m_nodes[from].get_index() != -1) && "<SparseGraph::getEdge>: invalid 'from' index");
	assert((to < m_nodes.size()) && (to >= 0) && (m_nodes[to].get_index() != -1) && "<SparseGraph::getEdge>: invalid 'to' index");

	for (auto currentEdge = m_edges[from].begin(); currentEdge != m_edges[from].end(); ++currentEdge)
	{
		if (currentEdge->get_to() == to)
		{
			return *currentEdge;
		}
	}

	assert(0 && "<SparseGraph::getEdge>: edge does not exist");
}

template<class nodeType, class edgeType>
edgeType& SparseGraph<nodeType, edgeType>::get_edge(int from, int to)
{
	assert((from < m_nodes.size()) && (from >= 0) && (m_nodes[from].get_index() != -1) && "<SparseGraph::getEdge>: invalid 'from' index");
	assert((to < m_nodes.size()) && (to >= 0) && (m_nodes[to].get_index() != -1) && "<SparseGraph::getEdge>: invalid 'to' index");

	for (auto currentEdge = m_edges[from].begin(); currentEdge != m_edges[from].end(); ++currentEdge)
	{
		if (currentEdge->get_to() == to)
		{
			return *currentEdge;
		}
	}

	assert(0 && "<SparseGraph::getEdge>: edge does not exist");
}

template<class nodeType, class edgeType>
int SparseGraph<nodeType, edgeType>::get_next_valid_node_index() const
{
	return m_next_valid_node_index;
}

template<class nodeType, class edgeType>
int SparseGraph<nodeType, edgeType>::add_node(nodeType node)
{
	if (node.get_index() < (int)m_nodes.size() && node.get_index() >= 0)
	{
		assert(m_nodes[node.get_index()].get_index() == -1 && "<SparseGraph::addNode>: attempting to add a node with duplicate id");

		m_nodes[node.get_index()] = node;

		return m_next_valid_node_index;
	}
	else
	{
		// if we add a node with id -1 we assign a valid id and add it to the graph
		if (node.get_index() == -1) node.set_index(m_next_valid_node_index);
		assert(node.get_index() == m_next_valid_node_index && "<SparseGraph::addNode>: invalid index");

		m_nodes.push_back(node);
		m_edges.push_back(std::vector<edgeType>());

		return m_next_valid_node_index++;
	}
}

template<class nodeType, class edgeType>
void SparseGraph<nodeType, edgeType>::remove_node(int index)
{
	assert(index < (int)m_nodes.size() && "<SparseGraph::removeNode>: invalid node index");

	m_nodes[index].setIndex(-1);

	if (!m_is_digraph)
	{
		for (auto currentEdge = m_edges[index].begin(); currentEdge != m_edges[index].end(); ++currentEdge)
		{
			for (auto edgeToRemove = m_edges[currentEdge->get_to()].begin();
				edgeToRemove != m_edges[currentEdge->get_to()].end();
				++edgeToRemove)
			{
				if (edgeToRemove->get_to() == index)
				{
					edgeToRemove = m_edges[currentEdge->get_to()].erase(edgeToRemove);
					break;
				}
			}
		}

		m_edges[index].clear();
	}
	else
	{
		cull_invalid_edges();
	}
}

template<class nodeType, class edgeType>
void SparseGraph<nodeType, edgeType>::add_edge(edgeType edge)
{
	assert(edge.get_from() < m_next_valid_node_index && edge.get_to() < m_next_valid_node_index && "<SparseGraph::addEdge>: invalid node index");

	if ((m_nodes[edge.get_to()].get_index() != -1) && (m_nodes[edge.get_from()].get_index() != -1))
	{
		if (is_edge_unique(edge.get_from(), edge.get_to()))
		{
			m_edges[edge.get_from()].push_back(edge);
		}

		if (!m_is_digraph)
		{
			if (is_edge_unique(edge.get_from(), edge.get_to()))
			{
				edgeType newEdge = edge;
				newEdge.set_from(edge.get_to());
				newEdge.set_to(edge.get_from());

				m_edges[edge.get_to()].push_back(newEdge);
			}
		}
	}
}

template<class nodeType, class edgeType>
void SparseGraph<nodeType, edgeType>::remove_edge(int from, int to)
{
	assert(from < (int)m_nodes.size() && to < (int)m_nodes.size() && "<SparseGraph::removeEdge>: invalid node index");

	if (!m_is_digraph)
	{
		for (auto currentEdge = m_edges[to].begin(); currentEdge != m_edges[to].end(); ++currentEdge)
		{
			if (currentEdge->get_to() == from)
			{
				currentEdge = m_edges[to].erase(currentEdge);
				break;
			}
		}
	}

	for (auto currentEdge = m_edges[from].begin(); currentEdge != m_edges[from].end(); ++currentEdge)
	{
		if (currentEdge->get_to() == to)
		{
			currentEdge = m_edges[from].erase(currentEdge);
			break;
		}
	}
}

template<class nodeType, class edgeType>
void SparseGraph<nodeType, edgeType>::set_edge_cost(int from, int to, float cost)
{
	assert(from < m_nodes.size() && to < m_nodes.size() && "<SparseGraph::setEdgeCost>: invalid node index");

	for (auto currentEdge = m_edges[from].begin(); currentEdge != m_edges[from].end(); ++currentEdge)
	{
		if (currentEdge->get_to() == to)
		{
			currentEdge->setCost(cost);
			break;
		}
	}
}

template<class nodeType, class edgeType>
int SparseGraph<nodeType, edgeType>::get_num_nodes() const
{
	return m_nodes.size();
}

template<class nodeType, class edgeType>
int SparseGraph<nodeType, edgeType>::get_num_active_nodes() const
{
	int count = 0;
	for (unsigned int n = 0; n < m_nodes.size(); ++n)
	{
		if (m_nodes[n].get_index() != -1)
		{
			count++;
		}
	}

	return count;
}

template<class nodeType, class edgeType>
int SparseGraph<nodeType, edgeType>::get_num_edges() const
{
	int total = 0;
	for (auto currentEdge = m_edges.begin(); currentEdge != m_edges.end(); currentEdge++)
	{
		total += currentEdge->size();
	}

	return total;
}

template<class nodeType, class edgeType>
bool SparseGraph<nodeType, edgeType>::is_digraph() const
{
	return m_is_digraph;
}

template<class nodeType, class edgeType>
bool SparseGraph<nodeType, edgeType>::is_empty() const
{
	return m_nodes.empty();
}

template<class nodeType, class edgeType>
bool SparseGraph<nodeType, edgeType>::is_node_present(int index) const
{
	if (index >= m_nodes.size() || m_nodes[index].get_index() == -1)
	{
		return false;
	}

	return true;
}

template<class nodeType, class edgeType>
bool SparseGraph<nodeType, edgeType>::is_edge_present(int from, int to) const
{
	if (is_node_present(from) && is_node_present(to))
	{
		for (auto currentEdge = m_edges[from].begin(); currentEdge != m_edges[from].end(); ++currentEdge)
		{
			if (currentEdge->get_to() == to)
			{
				return true;
			}
		}

		return false;
	}

	return false;
}

template<class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::clear()
{
	m_next_valid_node_index = 0; m_nodes.clear(); m_edges.clear();
}

template<class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::remove_edges()
{
	for (auto it = m_edges.begin(); it != m_edges.end(); ++it)
	{
		it->clear();
	}
}

template<class node_type, class edge_type>
bool SparseGraph<node_type, edge_type>::is_edge_unique(int from, int to) const
{
	for (auto currentEdge = m_edges[from].begin(); currentEdge != m_edges[from].end(); ++currentEdge)
	{
		if (currentEdge->get_to() == to)
		{
			return false;
		}
	}

	return true;
}

template<class node_type, class edge_type>
void SparseGraph<node_type, edge_type>::cull_invalid_edges()
{
	for (auto currentEdgeList = m_edges.begin(); currentEdgeList != m_edges.end(); ++currentEdgeList)
	{
		for (auto currentEdge = (*currentEdgeList).begin(); currentEdge != (*currentEdgeList).end(); ++currentEdge)
		{
			if (m_nodes[currentEdge->get_to()].get_index() == -1 ||
				m_nodes[currentEdge->get_from()].get_index() == -1)
			{
				currentEdge = (*currentEdgeList).erase(currentEdge);
			}
		}
	}
}
