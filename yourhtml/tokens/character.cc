#include <yourhtml/tokens/character.h>

namespace yourhtml {

character_t::character_t(const std::string &data_):
  token_t(token_t::CHARACTER),
  data(data_) {}

character_t::character_t(const char *data_):
  token_t(token_t::CHARACTER),
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
  if (isspace(that.data[0])) {
    strm << "<space>";
  } else {
    strm << that.data;
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

}
