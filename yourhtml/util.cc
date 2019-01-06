#include <iostream>
#include <sstream>
#include <yourhtml/util.h>

namespace yourhtml {

bool is_ascii_upper_alpha(int c) {
  return 0x0041 <= c && c <= 0x005A;
}

bool is_ascii_lower_alpha(int c) {
  return 0x0061 <= c && c <= 0x007A;
}

bool is_ascii_digit(int c) {
  return 0x0030 <= c && c <= 0x0039;
}

bool is_ascii_alpha(int c) {
  return is_ascii_lower_alpha(c) || is_ascii_upper_alpha(c);
}

bool is_ascii_alphanumeric(int c) {
  return is_ascii_alpha(c) || is_ascii_digit(c);
}

// A surrogate is a code point that is in the range U+D800
// to U+DFFF, inclusive.
bool is_surrogate(int codepoint) {
  return 0xD800 <= codepoint && codepoint <= 0xDFFF;
}

bool is_non_character(int codepoint) {
  if (0xFDD0 <= codepoint && codepoint <= 0xFDEF) {
    return true;
  } else {
    switch (codepoint) {
      case 0xFFFE:
      case 0xFFFF:
      case 0x1FFFE:
      case 0x1FFFF:
      case 0x2FFFE:
      case 0x2FFFF:
      case 0x3FFFE:
      case 0x3FFFF:
      case 0x4FFFE:
      case 0x4FFFF:
      case 0x5FFFE:
      case 0x5FFFF:
      case 0x6FFFE:
      case 0x6FFFF:
      case 0x7FFFE:
      case 0x7FFFF:
      case 0x8FFFE:
      case 0x8FFFF:
      case 0x9FFFE:
      case 0x9FFFF:
      case 0xAFFFE:
      case 0xAFFFF:
      case 0xBFFFE:
      case 0xBFFFF:
      case 0xCFFFE:
      case 0xCFFFF:
      case 0xDFFFE:
      case 0xDFFFF:
      case 0xEFFFE:
      case 0xEFFFF:
      case 0xFFFFE:
      case 0xFFFFF:
      case 0x10FFFF:
      case 0x10FFFE: {
        return true;
      }
    }
  }
  return false;
}

bool is_control_character(int codepoint) {
  return  (
    (0x0000 <= codepoint && codepoint <= 0x001F) ||
    (0x007F <= codepoint && codepoint <= 0x009F)
  );
}

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

bool is_ascii_ws(int c) {
  switch (c) {
    case 0x0009:
    case 0x000A:
    case 0x000C:
    case 0x000D:
    case 0x0020: {
      return true;
    }
  }
  return false;
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
