#include <iostream>
#include <sstream>
#include <yourhtml/util.h>

namespace yourhtml {

int codepoint(const std::string &u) {
  auto l = u.length();
  if (l < 1) {
    return -1;
  }
  unsigned char u0 = static_cast<unsigned char>(u[0]);
  if (u0 <= 127) {
    return u0;
  }
  if (l < 2) {
    return -1;
  }
  unsigned char u1 = static_cast<unsigned char>(u[1]);
  if (u0>=192 && u0<=223) {
    return (u0-192)*64 + (u1-128);
  }
  if (l < 3) {
    return -1;
  }
  unsigned char u2 = static_cast<unsigned char>(u[2]);
  if (u0>=224 && u0<=239) {
    return (u0-224) * 4096 + (u1-128)*64 + (u2-128);
  }
  if (l < 4) {
    return -1;
  }
  unsigned char u3 = static_cast<unsigned char>(u[3]);
  if (u0>=240 && u0<=247) {
    return (u0-240)*262144 + (u1-128)*4096 + (u2-128)*64 + (u3-128);
  }
  return -1;
}

std::string codepointhex(const std::string &u) {
  std::stringstream ss;
  std::string s;
  ss << std::showbase << std::hex << codepoint(u);
  ss >> s;
  return s;
}

std::string utf8chr(int cp) {
  char c[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
  if (cp<=0x7F) {
    c[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
    c[0] = static_cast<char>((cp>>6)+192);
    c[1] = static_cast<char>((cp&63) + 128);
  } else if (0xd800<=cp && cp<=0xdfff) {
    /* invalid block of utf8? */
  } else if (cp<=0xFFFF) {
    c[0] = static_cast<char>((cp>>12) + 224);
    c[1] = static_cast<char>(((cp>>6)&63)+128);
    c[2] = static_cast<char>((cp&63)+128);
  } else if (cp<=0x10FFFF) {
    c[0] = static_cast<char>((cp>>18) + 240);
    c[1] = static_cast<char>(((cp>>12)&63) + 128);
    c[2] = static_cast<char>(((cp>>6)&63)+128);
    c[3] = static_cast<char>((cp&63)+128);
  }
  return std::string(c);
}

} // yourhtml
