#pragma once

#include "math.hpp"

#include <list>
#include <vector>

template<class T>
struct Cell
{
	Cell(v2 pos, v2 dimension) : bounds{pos, pos + dimension} {}

	std::list<T>        members;
	math::Rect<float>   bounds;
};

template<class T>
class CellSpacePartition
{
public:
	CellSpacePartition(float worldSizeX, float worldSizeY, int numCellsX, int numCellsY, int numMaxMembers);

	void addMember(const T& member);
	void updateMember(const T& member, v2 oldPos);
	void calculateNeighbours(v2 targetPos, float queryRadius);


	T&   begin() { m_currentNeighbour = m_neighbours.begin(); return *m_currentNeighbour; }
	T&   next() { ++m_currentNeighbour; return *m_currentNeighbour; }
	bool end() { return (m_currentNeighbour == m_neighbours.end()) || (*m_currentNeighbour == 0); }

	void clearCells();

private:
	inline int positionToIndex(v2 position) const;

private:
	std::vector<Cell<T>> m_cells;
	std::vector<T>       m_neighbours;

	float m_worldSizeX, m_worldSizeY;
	int   m_numCellsX, m_numCellsY;
	float m_cellSizeX, m_cellSizeY;

	typename std::vector<T>::iterator   m_currentNeighbour;
};

#include "cell_space_partition.inl"
