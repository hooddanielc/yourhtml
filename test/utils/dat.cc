#include "dat.h"

namespace yourhtml {

/** error_t */
const char *dat_lexer_t::error_t::what() const noexcept {
  if (strm) {
    msg = strm->str();
    strm.reset();
  }
  return msg.c_str();
}

dat_lexer_t::error_t::error_t(const dat_lexer_t::pos_t &error_pos):
  strm(std::make_unique<std::ostringstream>()),
  sep_needed(false) {
  get_strm() << error_pos;
  end_section();
}

void dat_lexer_t::error_t::end_section() {
  sep_needed = true;
}

std::ostream &dat_lexer_t::error_t::get_strm() const {
  if (sep_needed) {
    (*strm) << "; ";
    sep_needed = false;
  }
  return *strm;
}

dat_lexer_t::error_t::~error_t() = default;

/** lexer_error_t */
dat_lexer_t::lexer_error_t::lexer_error_t(const dat_lexer_t *dat_lexer, const char *msg):
  error_t(dat_lexer->pos) { get_strm() << msg; }

dat_lexer_t::lexer_error_t::~lexer_error_t() = default;


/** ice_t */
/* Report the file and line at which we iced. */
dat_lexer_t::ice_t::ice_t(const dat_lexer_t::pos_t &ice_pos, const char *file, int line_number): error_t(ice_pos) {
  get_strm() << "internal compiler error";
  end_section();
  get_strm() << file << ", " << line_number;
}

dat_lexer_t::ice_t::~ice_t() = default;

void dat_lexer_t::print_tokens(const std::vector<token_t> &tokens) {
  for (const auto &token: tokens) {
    std::cout << token << std::endl;
  }
}

/** pos_t */
dat_lexer_t::pos_t::pos_t() noexcept: line_number(1), col_number(1) {}

void dat_lexer_t::pos_t::next_col() {
  ++col_number;
}

void dat_lexer_t::pos_t::next_line() {
  ++line_number;
  col_number = 1;
}

int dat_lexer_t::pos_t::get_line() const {
  return line_number;
}

int dat_lexer_t::pos_t::get_col() const {
  return col_number;
}

std::ostream &operator<<(std::ostream &strm, const dat_lexer_t::pos_t &that) {
  return strm
    << "line " << that.line_number
    << ", col " << that.col_number;
}

/** token_t */
dat_lexer_t::token_t::token_t(dat_lexer_t::token_t::kind_t kind_): kind(kind_) {}

dat_lexer_t::token_t::token_t(const dat_lexer_t::pos_t &pos_, dat_lexer_t::token_t::kind_t kind_): pos(pos_), kind(kind_) {}

dat_lexer_t::token_t::token_t(const dat_lexer_t::pos_t &pos_, dat_lexer_t::token_t::kind_t kind_, std::string &&text_):
  pos(pos_),
  kind(kind_),
  text(std::move(text_)) {}

std::string dat_lexer_t::token_t::get_desc(dat_lexer_t::token_t::kind_t kind) {
  switch(kind) {
    case TODO: return "TODO";
  }
  return "unknown";
}

dat_lexer_t::token_t::kind_t dat_lexer_t::token_t::get_kind() const {
  return kind;
}

std::string dat_lexer_t::token_t::get_text() const {
  return text;
}

std::string dat_lexer_t::token_t::get_name() const {
  return dat_lexer_t::token_t::get_desc(kind);
}

dat_lexer_t::pos_t dat_lexer_t::token_t::get_pos() const {
  return pos;
}

std::shared_ptr<dat_lexer_t::token_t> dat_lexer_t::token_t::make(dat_lexer_t::token_t::kind_t kind) {
  auto ptr = new token_t(kind);
  std::shared_ptr<dat_lexer_t::token_t> result(ptr);
  return result;
}

std::shared_ptr<dat_lexer_t::token_t> dat_lexer_t::token_t::make(const dat_lexer_t::pos_t &pos, dat_lexer_t::token_t::kind_t kind) {
  auto ptr = new token_t(pos, kind);
  std::shared_ptr<dat_lexer_t::token_t> result(ptr);
  return result;
}

std::shared_ptr<dat_lexer_t::token_t> dat_lexer_t::token_t::make(const dat_lexer_t::pos_t &pos, dat_lexer_t::token_t::kind_t kind, std::string &&text) {
  auto ptr = new token_t(pos, kind, std::move(text));
  std::shared_ptr<dat_lexer_t::token_t> result(ptr);
  return result;
}

/* Writes a human-readable dump of the token.  This is for debugging
 purposes only. In production, a user never sees tokens directly. */
std::ostream &operator<<(std::ostream &strm, const dat_lexer_t::token_t &that) {
  strm << that.pos << "; " << dat_lexer_t::token_t::get_desc(that.kind);
  if (!that.text.empty()) {
    strm << "; \"" << that.text << '"';
  }
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const dat_lexer_t::token_t *that) {
  strm << *that;
  return strm;
}

/** dat_lexer_t */
dat_lexer_t::dat_lexer_t(const char *next_cursor_):
  next_cursor(next_cursor_),
  is_ready(false),
  anchor(nullptr) {}

char dat_lexer_t::peek() const {
  if (!is_ready) {
    cursor = next_cursor;
    pos = next_pos;
    switch (*cursor) {
      case '\0': {
        break;
      }
      case '\n': {
        ++next_cursor;
        next_pos.next_line();
        break;
      }
      default: {
        ++next_cursor;
        next_pos.next_col();
      }
    }  // switch
    is_ready = true;
  }
  return *cursor;
}


char dat_lexer_t::pop() {
  char c = peek();
  is_ready = false;
  return c;
}

void dat_lexer_t::set_anchor() const {
  anchor_pos = pos;

  if (anchor) {
    throw ice_t(pos, __FILE__, __LINE__);
  }

  anchor = cursor;
}

std::string dat_lexer_t::pop_anchor() {
  if (!anchor) {
    throw ice_t(pos, __FILE__, __LINE__);
  }

  std::string text(anchor, static_cast<size_t>(cursor - anchor));
  anchor = nullptr;
  return text;
}

void dat_lexer_t::add_single_token(dat_lexer_t::token_t::kind_t kind) {
  set_anchor();
  pop();
  pop_anchor();
  //tokens.push_back(dat_lexer_t::token_t::make(anchor_pos, kind));
  tokens.push_back(dat_lexer_t::token_t::make(anchor_pos, kind));
}

std::vector<std::shared_ptr<dat_lexer_t::token_t>> dat_lexer_t::lex() {
  enum {
    start,
  } state = start;
  bool go = true;
  do {
    char c = peek();
    switch (state) {
      case start: {
        switch (c) {
          case '\0': {
            set_anchor();
            go = false;
            break;
          }
          default: {
            add_single_token(dat_lexer_t::token_t::TODO);
          }
        }
      }
    }
  } while (go);
  return tokens;
}

}   // yourhtml
