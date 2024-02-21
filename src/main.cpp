#include "application.hpp"
#include "intersection_tests.hpp"

auto test() -> void
{
	auto const ls = v2(25, -25);
	auto const le = v2(25, 25);
	auto const p = v2(25.5, 15);
	auto const r = 1.f;
	auto dist = distance_point_to_line_segment(p, ls, le);
	std::cout << dist << std::endl;
}

auto main(int argc, char** argv) -> int
{
    std::srand(0);
    Application app(argc, argv);
    app.run();
    return EXIT_SUCCESS;
}
