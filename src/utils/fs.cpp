#include "fs.h"

std::string nerds::fs::read_file_contents(std::string file_path) {
    std::ifstream t(file_path);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    return str;
}

json nerds::fs::get_json_from_file(std::string file_path) {
    std::ifstream t(file_path);
    assert(t.is_open());
    json j;
    t >> j;
    return j;
}