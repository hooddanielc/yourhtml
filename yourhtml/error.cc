#include "error.h"

namespace yourhtml {

const char *error_t::what() const noexcept {
  if (strm) {
    msg = strm->str();
    strm.reset();
  }
  return msg.c_str();
}

error_t::error_t(const pos_t &pos_):
  strm(std::make_unique<std::ostringstream>()),
  sep_needed(false),
  pos(pos_) {
  get_strm() << pos_;
  end_section();
}

void error_t::end_section() {
  sep_needed = true;
}

pos_t error_t::get_pos() const {
  return pos;
}

std::ostream &error_t::get_strm() const {
  if (sep_needed) {
    (*strm) << "; ";
    sep_needed = false;
  }
  return *strm;
}

error_t::~error_t() = default;

}  // yourcss
