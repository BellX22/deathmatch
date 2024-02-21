#include "wall.hpp"

Wall::Wall(v2 start, v2 end)
    : m_start(start), m_end(end)
{
}

auto Wall::start() const -> v2
{
    return m_start;
}

auto Wall::end() const -> v2
{
    return m_end;
}

auto Wall::normal() const -> v2
{
    auto tmp = normalize(direction());
    return {-tmp.y, tmp.x};
}

auto Wall::direction() const -> v2
{
    return m_end - m_start;
}

auto Wall::center() const -> v2
{
    return (m_start + m_end) / 2.f;
}

auto Wall::serialize(std::ostream &out) -> void
{
    out << m_start.x << " " << m_start.y << " "
        << m_end.x << " " << m_end.y << std::endl;
}

auto Wall::deserialize(std::istream &in) -> void
{
    in >> m_start.x >> m_start.y >> m_end.x >> m_end.y;
}
