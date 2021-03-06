#include "tokens/character.h"
#include "tokens/tag.h"
#include "token.h"

namespace yourhtml {

class character_t;

class tag_t;

token_t::token_t(token_t::kind_t kind_):
  kind(kind_){}

token_t::token_t(const pos_t &pos_, token_t::kind_t kind_):
  pos(pos_),
  kind(kind_) {}

token_t::~token_t() = default;

std::string token_t::get_desc(token_t::kind_t kind) {
  switch(kind) {
    case DOCTYPE: return "DOCTYPE";
    case START_TAG: return "START_TAG";
    case END_TAG: return "END_TAG";
    case COMMENT: return "COMMENT";
    case CHARACTER: return "CHARACTER";
    case END_OF_FILE: return "END_OF_FILE";
  }
}

token_t::kind_t token_t::get_kind() const {
  return kind;
}

std::string token_t::get_description() const {
  return token_t::get_desc(kind);
}

pos_t token_t::get_pos() const {
  return pos;
}

std::shared_ptr<token_t> token_t::make(token_t::kind_t kind) {
  return std::make_shared<token_t>(kind);
}

std::shared_ptr<token_t> token_t::make(const pos_t &pos, token_t::kind_t kind) {
  return std::make_shared<token_t>(pos, kind);
}

/* Writes a human-readable dump of the token.  This is for debugging
 purposes only. In production, a user never sees tokens directly. */
std::ostream &operator<<(std::ostream &strm, const token_t &that) {
  strm << that.pos << "; " << token_t::get_desc(that.kind);
  if (!that.text.empty()) {
    strm << "; \"" << that.text << '"';
  }
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const token_t *that) {
  switch (that->get_kind()) {
    case token_t::CHARACTER: {
      strm << dynamic_cast<const character_t*>(that);
      break;
    }
    case token_t::END_TAG:
    case token_t::START_TAG: {
      strm << dynamic_cast<const tag_t*>(that);
      break;
    }
    case token_t::DOCTYPE:
    case token_t::COMMENT:
    case token_t::END_OF_FILE: {
      strm << *that;
      break;
    }
  }
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const std::vector<token_t*> &that) {
  for (auto item: that) {
    strm << item << " ";
  }
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const std::vector<std::shared_ptr<token_t>> &that) {
  for (auto item: that) {
    strm << item.get() << " ";
  }
  return strm;
}

}   // yourcss
