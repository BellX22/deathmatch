#include "game_map.hpp"

auto GameMap::add_wall(v2 start, v2 end) -> index_t
{
    m_walls.push_back(Wall(start, end));
    return m_walls.size();
}

auto GameMap::add_door(index_t wall1, index_t wall2) -> void
{
    m_doors.push_back(Door(this, wall1, wall2));
}

auto GameMap::calc_map_bounds() -> void
{
    v2 max{std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};
    v2 min{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    for(auto&& wall : m_walls)
    {
        auto s = wall.get_start();
        auto e = wall.get_end();

        if(s.x < min.x)
            min.x = s.x;
        if(s.y < min.y)
            min.y = s.y;
        if(e.x < min.x)
            min.x = e.x;
        if(e.y < min.y)
            min.y = e.y;

        if(s.x > max.x)
            max.x = s.x;
        if(s.y > max.y)
            max.y = s.y;
        if(e.x > max.x)
            max.x = e.x;
        if(e.y > max.y)
            max.y = e.y;
    }
    m_min = min;
    m_max = max;
}
