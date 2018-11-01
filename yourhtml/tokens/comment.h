#pragma once
#include <yourhtml/token.h>

namespace yourhtml {

class comment_t: public token_t {

public:

  comment_t(const pos_t &pos);

  virtual ~comment_t();

  comment_t(const comment_t&) = default;

  comment_t(comment_t&&) = default;

  comment_t& operator=(const comment_t&) = default;

  comment_t& operator=(comment_t&&) = default;

};

}
