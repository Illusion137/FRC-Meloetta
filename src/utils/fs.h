#pragma once

#include <nlohmann/json.hpp>

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <streambuf>

using namespace nlohmann;

namespace nerds {
    namespace fs {
        //https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
        extern std::string read_file_contents(std::string file_path);
        extern json get_json_from_file(std::string file_path);
    }
}