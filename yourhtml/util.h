#pragma once

#include <string>

namespace yourhtml {

bool is_ascii_upper_alpha(int c);
bool is_ascii_lower_alpha(int c);
bool is_ascii_digit(int c);
bool is_ascii_alpha(int c);
bool is_ascii_alphanumeric(int c);
bool is_ascii_ws(int c);
bool is_non_character(int codepoint);
bool is_control_character(int codepoint);
bool is_surrogate(int codepoint);
int codepoint(const std::string &u);
std::string codepointhex(const std::string &u);
std::string utf8chr(int cp);

}
