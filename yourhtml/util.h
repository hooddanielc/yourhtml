#pragma once

#include <string>

namespace yourhtml {

bool is_ascii_ws(char c);
bool is_non_character(int codepoint);
bool is_control_character(int codepoint);
bool is_surrogate(int codepoint);
int codepoint(const std::string &u);
std::string codepointhex(const std::string &u);
std::string utf8chr(int cp);

}
