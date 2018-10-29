#pragma once
#include <unordered_map>
#include <string>

namespace yourhtml {

std::unordered_map<std::string, std::string> &get_character_reference_lookup_map();

std::string lookup_characters(const std::string &key);

}
