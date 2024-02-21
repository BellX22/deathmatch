#pragma once
#include <map>
#include <string>
#include <filesystem>

class IniFile {
public:
    IniFile() = default;

    auto load(std::filesystem::path const &path) -> void;
    auto save(std::filesystem::path const &path) -> void;
    auto get_string(std::string const &key, std::string const default_value) -> std::string const&;
    auto get_int(std::string const &key, int default_value) -> int;
    auto get_bool(std::string const &key, bool default_value) -> bool;

private:
    std::filesystem::path m_filename;
    std::map<std::string, std::string> m_key_value;
};
