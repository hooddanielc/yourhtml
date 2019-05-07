#include <yourhtml/tokens/character.h>

namespace yourhtml {

character_t::character_t(const std::string &data_):
  token_t(token_t::CHARACTER),
  data(data_) {}

character_t::character_t(const char *data_):
  token_t(token_t::CHARACTER),
  data(data_) {}

character_t::character_t(const pos_t &pos, const std::string &data_):
  token_t(pos, token_t::CHARACTER),
  data(data_) {}

character_t::character_t(const pos_t &pos, char c):
  token_t(pos, token_t::CHARACTER),
  data(new char[2]{c, '\0'}) {}

character_t::character_t(const pos_t &pos, const char *data_):
  token_t(pos, token_t::CHARACTER),
  data(data_) {}

character_t::~character_t() = default;

std::string character_t::get_data() const {
  return data;
}

std::ostream &operator<<(std::ostream &strm, const character_t &that) {
  strm << that.pos << "; " << character_t::get_desc(that.kind);

  strm << "; \"";
  for (auto c: that.data) {
    switch (c) {
      case '\r': {
        strm << "\\r";
        break;
      }
      case '\n': {
        strm << "\\n";
        break;
      }
      case 0x000B: {
        strm << "<tab>";
        break;
      }
      default: {
        if (isspace(c)) {
          strm << "<space>";
        } else {
          strm << c;
        }
      }
    }
  }
  strm << "\"";

  return strm;
}

std::ostream &operator<<(std::ostream &strm, const character_t *that) {
  strm << *that;
  return strm;
}

bool character_t::is_space() const {
  return isspace(data[0]);
}

bool operator==(const character_t &lhs, const character_t &rhs) {
  if (lhs.get_data() != rhs.get_data()) {
    return false;
  }
  return true;
}

}
