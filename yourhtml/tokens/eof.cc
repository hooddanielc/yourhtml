#include <yourhtml/tokens/eof.h>

namespace yourhtml {

eof_t::eof_t(const pos_t &pos): token_t(pos, token_t::END_OF_FILE) {}

eof_t::~eof_t() = default;

}
