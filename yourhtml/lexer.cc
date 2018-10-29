#include "lexer.h"

namespace yourhtml {

lexer_t::lexer_error_t::lexer_error_t(const lexer_t *lexer, const char *msg):
  error_t(lexer->pos) { get_strm() << msg; }

lexer_t::lexer_error_t::~lexer_error_t() = default;

void lexer_t::print_tokens(const std::vector<token_t> &tokens) {
  for (const auto &token: tokens) {
    std::cout << token << std::endl;
  }
}

void lexer_t::print_tokens(const std::vector<std::shared_ptr<token_t>> &tokens) {
  for (const auto &token: tokens) {
    std::cout << (*token) << std::endl;
  }
}

lexer_t::lexer_t(const char *next_cursor_):
  next_cursor(next_cursor_),
  is_ready(false),
  anchor(nullptr),
  state(data),
  return_state(idle) {}

char lexer_t::peek() const {
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

char lexer_t::reset_cursor(const char *saved_cursor) {
  is_ready = false;
  next_cursor = saved_cursor;
  char c = peek();
  return c;
}

char lexer_t::pop() {
  char c = peek();
  is_ready = false;
  return c;
}

void lexer_t::set_anchor() const {
  anchor_pos = pos;

  if (anchor) {
    throw ice_t(pos, __FILE__, __LINE__);
  }

  anchor = cursor;
}

std::string lexer_t::pop_anchor() {
  if (!anchor) {
    for (const auto &token: tokens) {
      std::cout << token << std::endl;
    }
    throw ice_t(pos, __FILE__, __LINE__);
  }

  std::string text(anchor, static_cast<size_t>(cursor - anchor));
  anchor = nullptr;
  return text;
}

void lexer_t::add_single_token(token_t::kind_t kind) {
  set_anchor();
  pop();
  pop_anchor();
  tokens.push_back(token_t::make(anchor_pos, kind));
}

lexer_t::state_t lexer_t::pop_state() {
  auto current_state = return_state;
  if (current_state == idle) {
    throw ice_t(pos, __FILE__, __LINE__);
  }
  return_state = idle;
  return current_state;
}

void lexer_t::push_state(state_t return_state_) {
  if (return_state == idle) {
    throw ice_t(pos, __FILE__, __LINE__);
  }
  return_state = return_state_;
}

std::vector<std::shared_ptr<token_t>> lexer_t::lex() {
  bool go = true;
  do {
    char c = peek();
    switch (state) {
      case idle: {
        throw ice_t(pos, __FILE__, __LINE__);
      }
      case data: {
        break;
      }
      case rcdata: {
        break;
      }
      case rawtext: {
        break;
      }
      case script_data: {
        break;
      }
      case plaintext: {
        break;
      }
      case tag_open: {
        break;
      }
      case end_tag_open: {
        break;
      }
      case tag_name: {
        break;
      }
      case rcdata_less_than_sign: {
        break;
      }
      case rcdata_end_tag_open: {
        break;
      }
      case rcdata_end_tag_name: {
        break;
      }
      case rawtext_less_than_sign: {
        break;
      }
      case rawtext_end_tag_open: {
        break;
      }
      case rawtext_end_tag_name: {
        break;
      }
      case script_data_less_than_sign: {
        break;
      }
      case script_data_end_tag_open: {
        break;
      }
      case script_data_end_tag_name: {
        break;
      }
      case script_data_escape_start: {
        break;
      }
      case script_data_escape_start_dash: {
        break;
      }
      case script_data_escaped: {
        break;
      }
      case script_data_escaped_dash: {
        break;
      }
      case script_data_escaped_dash_dash: {
        break;
      }
      case script_data_escaped_less_than_sign: {
        break;
      }
      case script_data_escaped_end_tag_open: {
        break;
      }
      case script_data_escaped_end_tag_name: {
        break;
      }
      case script_data_double_escape_start: {
        break;
      }
      case script_data_double_escaped: {
        break;
      }
      case script_data_double_escaped_dash: {
        break;
      }
      case script_data_double_escaped_dash_dash: {
        break;
      }
      case script_data_double_escaped_less_than_sign: {
        break;
      }
      case script_data_double_escape_end: {
        break;
      }
      case before_attribute_name: {
        break;
      }
      case attribute_name: {
        break;
      }
      case after_attribute_name: {
        break;
      }
      case before_attribute_value: {
        break;
      }
      case attribute_value_double_quoted: {
        break;
      }
      case attribute_value_single_quoted: {
        break;
      }
      case attribute_value_unquoted: {
        break;
      }
      case after_attribute_value_quoted: {
        break;
      }
      case self_closing_start_tag: {
        break;
      }
      case bogus_comment: {
        break;
      }
      case markup_declaration_open: {
        break;
      }
      case comment_start: {
        break;
      }
      case comment_start_dash: {
        break;
      }
      case comment: {
        break;
      }
      case comment_less_then_sign: {
        break;
      }
      case comment_less_then_sign_bang: {
        break;
      }
      case comment_less_then_sign_bang_dash: {
        break;
      }
      case comment_less_then_sign_bang_dash_dash: {
        break;
      }
      case comment_end_dash_state: {
        break;
      }
      case comment_end_state: {
        break;
      }
      case comment_end_bang: {
        break;
      }
      case doctype: {
        break;
      }
      case before_doctype_name: {
        break;
      }
      case doctype_name: {
        break;
      }
      case after_doctype_name: {
        break;
      }
      case after_doctype_public_keyword: {
        break;
      }
      case before_doctype_public_identifier: {
        break;
      }
      case doctype_public_identifier_double_quoted: {
        break;
      }
      case doctype_public_identifier_single_quoted: {
        break;
      }
      case after_doctype_public_identifier: {
        break;
      }
      case between_doctype_public_and_system_identifiers: {
        break;
      }
      case after_doctype_system_keyword: {
        break;
      }
      case before_doctype_system_identifier: {
        break;
      }
      case doctype_system_identifier_double_quoted: {
        break;
      }
      case doctype_system_identifier_single_quoted: {
        break;
      }
      case after_doctype_system_identifier: {
        break;
      }
      case bogus_doctype: {
        break;
      }
      case cdata_section: {
        break;
      }
      case cdata_section_bracket: {
        break;
      }
      case cdata_section_end: {
        break;
      }
      case character_reference_state: {
        break;
      }
      case named_character_reference: {
        break;
      }
      case ambiguous_ampersand: {
        break;
      }
      case numeric_character_reference: {
        break;
      }
      case hexadecimal_character_reference_start: {
        break;
      }
      case decimal_character_reference_start: {
        break;
      }
      case hexadecimal_character_reference: {
        break;
      }
      case decimal_character_reference: {
        break;
      }
      case numeric_character_reference_end: {
        break;
      }
    }
  } while (go);
  return tokens;
}

}   // yourcss
