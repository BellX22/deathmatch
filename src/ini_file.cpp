#include "ini_file.hpp"
#include <fstream>

auto IniFile::load(std::filesystem::path const &path) -> void
{
    std::ifstream in(path);
    if(!in.is_open()) {
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(in, line)) {
        if(line.empty())
            continue;

        std::stringstream ss(line);
        std::string key;
        std::string value;
        std::getline(ss, key, '=');
        std::getline(ss, value);

        m_key_value[key] = value;
    }
}

auto IniFile::save(std::filesystem::path const &path) -> void
{
}
