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
  anchor(nullptr) {}

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

std::vector<std::shared_ptr<token_t>> lexer_t::lex() {
  enum {
    data,
    rcdata,
    rawtext,
    script_data,
    plaintext,
    tag_open,
    end_tag_open,
    tag_name,
    rcdata_less_than_sign,
    rcdata_end_tag_open,
    rcdata_end_tag_name,
    rawtext_less_than_sign,
    rawtext_end_tag_open,
    rawtext_end_tag_name,
    script_data_less_than_sign,
    script_data_end_tag_open,
    script_data_end_tag_name,
    script_data_escape_start,
    script_data_escape_start_dash,
    script_data_escaped,
    script_data_escaped_dash,
    script_data_escaped_dash_dash,
    script_data_escaped_less_than_sign,
    script_data_escaped_end_tag_open,
    script_data_escaped_end_tag_name,
    script_data_double_escape_start,
    script_data_double_escaped,
    script_data_double_escaped_dash,
    script_data_double_escaped_dash_dash,
    script_data_double_escaped_less_than_sign,
    script_data_double_escape_end,
    before_attribute_name,
    attribute_name,
    after_attribute_name,
    before_attribute_value,
    attribute_value_double_quoted,
    attribute_value_single_quoted,
    attribute_value_unquoted,
    after_attribute_value_quoted,
    self_closing_start_tag,
    bogus_comment,
    markup_declaration_open,
    comment_start,
    comment_start_dash,
    comment,
    comment_less_then_sign,
    comment_less_then_sign_bang,
    comment_less_then_sign_bang_dash,
    comment_less_then_sign_bang_dash_dash,
    comment_end_dash_state,
    comment_end_state,
    comment_end_bang,
    doctype,
    before_doctype_name,
    doctype_name,
    after_doctype_name,
    after_doctype_public_keyword,
    before_doctype_public_identifier,
    doctype_public_identifier_double_quoted,
    doctype_public_identifier_single_quoted,
    after_doctype_public_identifier,
    between_doctype_public_and_system_identifiers,
    after_doctype_system_keyword,
    before_doctype_system_identifier,
    doctype_system_identifier_double_quoted,
    doctype_system_identifier_single_quoted,
    after_doctype_system_identifier,
    bogus_doctype,
    cdata_section,
    cdata_section_bracket,
    cdata_section_end,
    character_reference_state,
    named_character_reference,
    ambiguous_ampersand,
    numeric_character_reference,
    hexadecimal_character_reference_start,
    decimal_character_reference_start,
    hexadecimal_character_reference,
    decimal_character_reference,
    numeric_character_reference_end,
  } state = data;
  bool go = true;
  do {
    char c = peek();
    switch (state) {
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
