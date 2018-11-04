#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>

namespace yourhtml {

std::unordered_map<std::string, std::vector<unsigned int>> &get_character_reference_lookup_map();

std::vector<unsigned int> lookup_characters(const std::string &key);

std::string convert_codepoints_to_utf8(const std::vector<unsigned int> &codepoints);

}
