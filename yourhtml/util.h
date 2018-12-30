#pragma once

#include <string>

namespace yourhtml {

int codepoint(const std::string &u);
std::string codepointhex(const std::string &u);
std::string utf8chr(int cp);

}
