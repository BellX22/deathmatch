
template<class T>
CellSpacePartition<T>::CellSpacePartition(float worldSizeX, float worldSizeY, int numCellsX, int numCellsY, int numMaxMembersToPartition) :
	m_worldSizeX(worldSizeX),
	m_worldSizeY(worldSizeY),
	m_numCellsX(numCellsX),
	m_numCellsY(numCellsY),
	m_neighbours(numMaxMembersToPartition, T())
{
	m_cellSizeX = worldSizeX / numCellsX;
	m_cellSizeY = worldSizeY / numCellsY;

	for (int y = 0; y < m_numCellsY; y++)
	{
		for (int x = 0; x < m_numCellsX; x++)
		{
			//remember x -> right, y -> down
			float posX = x * m_cellSizeX;
			float posY = y * m_cellSizeY;
			float width = m_cellSizeX;
			float height = m_cellSizeY;

			m_cells.push_back(Cell<T>(v2(posX, posY), v2(width, height)));
		}
	}
}

template<class T>
void CellSpacePartition<T>::addMember(const T& member)
{
	int size = m_cells.size();
	int index = positionToIndex(member->getPosition());

	m_cells[index].members.push_back(member);
}

template<class T>
void CellSpacePartition<T>::updateMember(const T& member, v2 oldPos)
{
	int oldIndex = positionToIndex(oldPos);
	int newIndex = positionToIndex(member->getPosition());

	if (newIndex == oldIndex) return;

	m_cells[oldIndex].members.remove(member);
	m_cells[newIndex].members.push_back(member);
}

template<class T>
void CellSpacePartition<T>::calculateNeighbours(v2 targetPos, float queryRadius)
{
	auto currentNeighbour = m_neighbours.begin();

	Rectangle2D queryBox(v2(targetPos.x - queryRadius / 2, targetPos.y - queryRadius / 2), v2(queryRadius, queryRadius));

	for (auto currentCell = m_cells.begin(); currentCell != m_cells.end(); ++currentCell)
	{
		if (currentCell->bounds.overlaps(queryBox) && !currentCell->members.empty())
		{
			for (auto it = currentCell->members.begin(); it != currentCell->members.end(); ++it)
			{
				if (v2::distanceSquare((*it)->getPosition(), targetPos) < (queryRadius * queryRadius))
				{
					*currentNeighbour++ = *it;
				}
			}
		}
	}

	*currentNeighbour = 0;
}

template<class T>
void CellSpacePartition<T>::clearCells()
{
	for (auto it = m_cells.begin(); it != m_cells.end(); ++it)
	{
		it->members.clear();
	}
}

template<class T>
int CellSpacePartition<T>::positionToIndex(v2 position) const
{
	int index = (int)(m_numCellsX * position.x / m_worldSizeX) + ((int)((m_numCellsY)* position.y / m_worldSizeY) * m_numCellsX);

	if (index > (int)m_cells.size() - 1)
		index = (int)m_cells.size() - 1;

	return index;
}