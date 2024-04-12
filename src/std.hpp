#pragma once

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

// all c++ headers
//__cplusplus 199711L (C++98 or C++03)
//__cplusplus 201103L (C++11)
//__cplusplus 201402L (C++14)
//__cplusplus 201703L (C++17)
//__cplusplus 202002L (C++20)

// -- UTILITY LIBRARIES --
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <functional>
#include <typeinfo>
#include <utility>
#include <chrono> // c++11
#include <initializer_list> // c++11
#include <tuple> // c++11
#include <type_traits> // c++11
#include <typeindex> // c++11
#include <any> // c++17
#include <optional> // c++17
#include <variant> // c++17
#include <compare> // c++20
#include <concepts> // c++20
#include <coroutine> // c++20
#include <source_location> // c++20
#include <version> // c++20
//#include <expected> // c++23

// -- DYNAMIC MEMORY MANAGEMENT --
#include <memory>
#include <new>
#include <scoped_allocator>
#include <memory_resource> // c++ 17

// -- NUMERIC LIMITS --
#include <cfloat>
#include <climits>
#include <limits>
#include <cstdint> // c++11
#include <cinttypes> // c++11
//#include <stdfloat> // c++ 23

// -- ERROR HANDLING --
#include <cassert>
#include <cerrno>
#include <exception>
#include <stdexcept>
#include <system_error> // c++11
//#include <stack_trace> // c++23

// -- STRINGS LIBRARY --
#include <cctype>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <string>
#include <cuchar> // c++11
#include <charconv> // c++17
#include <string_view> // c++17
//#include <format> // c++20

// -- CONTAINERS LIBRARY --
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <array> // c++11
#include <unordered_map> // c++11
#include <unordered_set> // c++11
#include <forward_list> // c++11
//#include <span> // c++20
//#include <mdspan> // c++23
//#include <flat_map> // c++23
//#include <flat_set> // c++23

// -- ITERATOR LIBRARY --
#include <iterator>

// -- RANGES LIBRARY -- 
#include <ranges> // c++20
//#include <generator> // c++23

// -- ALGORITHM LIBRARY --
#include <algorithm>
#include <execution> // c++17

// -- NUMERICS LIBRARY --
#include <cmath>
#include <complex>
#include <valarray>
#include <numeric>
#include <cfenv> // c++11
#include <random> // c++11
#include <ratio> // c++11
#include <bit> // c++20
#include <numbers> // c++20

// -- LOCALIZATION LIBRARY --
#include <clocale>
#include <locale>

// -- I/O LIBRARY --
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <syncstream> // c++20
//#include <print> // c++23
//#include <spanstream> // c++23

// -- FILESYSTEM LIBRARY --
#include <filesystem> // c++17

// -- REGEX LIBRARY --
#include <regex> // c++11

// -- ATOMIC LIBRARY --
#include <atomic> // c++11

// -- THREAD SUPPORT LIBRARY --
#include <condition_variable> // c++11
#include <future> // c++11
#include <mutex> // c++11
#include <thread> // c++11
#include <shared_mutex> // c++14
#include <latch> // c++20
#include <barrier> // c++20
#include <stop_token> // c++20
#include <semaphore> // c++20

using namespace std::literals::chrono_literals;
using namespace std::literals::string_view_literals;
using namespace std::literals::string_literals;
namespace fs = std::filesystem;
