#include "pos.h"

namespace yourhtml {

pos_t::pos_t() noexcept: line_number(1), col_number(1) {}

pos_t::pos_t(int line_number_, int col_number_) noexcept:
  line_number(line_number_),
  col_number(col_number_) {}

void pos_t::next_col() {
  ++col_number;
}

void pos_t::next_line() {
  ++line_number;
  col_number = 1;
}

int pos_t::get_line() const {
  return line_number;
}

int pos_t::get_col() const {
  return col_number;
}

std::ostream &operator<<(std::ostream &strm, const pos_t &that) {
  return strm
    << "line " << that.line_number
    << ", col " << that.col_number;
}

}
