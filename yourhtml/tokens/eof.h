#pragma once
#include <yourhtml/token.h>

namespace yourhtml {

class eof_t: public token_t {

public:

  eof_t(const pos_t &pos);

  virtual ~eof_t();

  eof_t(const eof_t&) = default;

  eof_t(eof_t&&) = default;

  eof_t& operator=(const eof_t&) = default;

  eof_t& operator=(eof_t&&) = default;

};  // eof_t

}
