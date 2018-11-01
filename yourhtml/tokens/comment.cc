#include <yourhtml/tokens/comment.h>

namespace yourhtml {

comment_t::comment_t(const pos_t &pos):
  token_t(pos, token_t::COMMENT) {}

comment_t::~comment_t() = default;

}
