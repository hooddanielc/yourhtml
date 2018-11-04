#include "lexer.h"

namespace yourhtml {

lexer_t::lexer_error_t::lexer_error_t(const lexer_t *lexer, const char *msg):
  error_t(lexer->pos) { get_strm() << msg; }

lexer_t::lexer_error_t::~lexer_error_t() = default;

lexer_t::~lexer_t() = default;

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

std::string lexer_t::get_state_name(state_t state_) {
  switch (state_) {
    case idle: return "idle";
    case data: return "data";
    case rcdata: return "rcdata";
    case rawtext: return "rawtext";
    case script_data: return "script_data";
    case plaintext: return "plaintext";
    case tag_open: return "tag_open";
    case end_tag_open: return "end_tag_open";
    case tag_name: return "tag_name";
    case rcdata_less_than_sign: return "rcdata_less_than_sign";
    case rcdata_end_tag_open: return "rcdata_end_tag_open";
    case rcdata_end_tag_name: return "rcdata_end_tag_name";
    case rawtext_less_than_sign: return "rawtext_less_than_sign";
    case rawtext_end_tag_open: return "rawtext_end_tag_open";
    case rawtext_end_tag_name: return "rawtext_end_tag_name";
    case script_data_less_than_sign: return "script_data_less_than_sign";
    case script_data_end_tag_open: return "script_data_end_tag_open";
    case script_data_end_tag_name: return "script_data_end_tag_name";
    case script_data_escape_start: return "script_data_escape_start";
    case script_data_escape_start_dash: return "script_data_escape_start_dash";
    case script_data_escaped: return "script_data_escaped";
    case script_data_escaped_dash: return "script_data_escaped_dash";
    case script_data_escaped_dash_dash: return "script_data_escaped_dash_dash";
    case script_data_escaped_less_than_sign: return "script_data_escaped_less_than_sign";
    case script_data_escaped_end_tag_open: return "script_data_escaped_end_tag_open";
    case script_data_escaped_end_tag_name: return "script_data_escaped_end_tag_name";
    case script_data_double_escape_start: return "script_data_double_escape_start";
    case script_data_double_escaped: return "script_data_double_escaped";
    case script_data_double_escaped_dash: return "script_data_double_escaped_dash";
    case script_data_double_escaped_dash_dash: return "script_data_double_escaped_dash_dash";
    case script_data_double_escaped_less_than_sign: return "script_data_double_escaped_less_than_sign";
    case script_data_double_escape_end: return "script_data_double_escape_end";
    case before_attribute_name: return "before_attribute_name";
    case attribute_name: return "attribute_name";
    case after_attribute_name: return "after_attribute_name";
    case before_attribute_value: return "before_attribute_value";
    case attribute_value_double_quoted: return "attribute_value_double_quoted";
    case attribute_value_single_quoted: return "attribute_value_single_quoted";
    case attribute_value_unquoted: return "attribute_value_unquoted";
    case after_attribute_value_quoted: return "after_attribute_value_quoted";
    case self_closing_start_tag: return "self_closing_start_tag";
    case bogus_comment: return "bogus_comment";
    case markup_declaration_open: return "markup_declaration_open";
    case comment_start: return "comment_start";
    case comment_start_dash: return "comment_start_dash";
    case comment: return "comment";
    case comment_less_than_sign: return "comment_less_than_sign";
    case comment_less_than_sign_bang: return "comment_less_than_sign_bang";
    case comment_less_than_sign_bang_dash: return "comment_less_than_sign_bang_dash";
    case comment_less_than_sign_bang_dash_dash: return "comment_less_than_sign_bang_dash_dash";
    case comment_end_dash: return "comment_end_dash";
    case comment_end: return "comment_end";
    case comment_end_bang: return "comment_end_bang";
    case doctype: return "doctype";
    case before_doctype_name: return "before_doctype_name";
    case doctype_name: return "doctype_name";
    case after_doctype_name: return "after_doctype_name";
    case after_doctype_public_keyword: return "after_doctype_public_keyword";
    case before_doctype_public_identifier: return "before_doctype_public_identifier";
    case doctype_public_identifier_double_quoted: return "doctype_public_identifier_double_quoted";
    case doctype_public_identifier_single_quoted: return "doctype_public_identifier_single_quoted";
    case after_doctype_public_identifier: return "after_doctype_public_identifier";
    case between_doctype_public_and_system_identifiers: return "between_doctype_public_and_system_identifiers";
    case after_doctype_system_keyword: return "after_doctype_system_keyword";
    case before_doctype_system_identifier: return "before_doctype_system_identifier";
    case doctype_system_identifier_double_quoted: return "doctype_system_identifier_double_quoted";
    case doctype_system_identifier_single_quoted: return "doctype_system_identifier_single_quoted";
    case after_doctype_system_identifier: return "after_doctype_system_identifier";
    case bogus_doctype: return "bogus_doctype";
    case cdata_section: return "cdata_section";
    case cdata_section_bracket: return "cdata_section_bracket";
    case cdata_section_end: return "cdata_section_end";
    case character_reference: return "character_reference";
    case named_character_reference: return "named_character_reference";
    case ambiguous_ampersand: return "ambiguous_ampersand";
    case numeric_character_reference: return "numeric_character_reference";
    case hexadecimal_character_reference_start: return "hexadecimal_character_reference_start";
    case decimal_character_reference_start: return "decimal_character_reference_start";
    case hexadecimal_character_reference: return "hexadecimal_character_reference";
    case decimal_character_reference: return "decimal_character_reference";
    case numeric_character_reference_end: return "numeric_character_reference_end";
  }
}

lexer_t::lexer_t(const char *next_cursor_):
  next_cursor(next_cursor_),
  is_ready(false),
  anchor(nullptr),
  state(data),
  return_state(idle),
  current_tag_self_closing(false) {}

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

lexer_t::state_t lexer_t::pop_state() {
  auto current_state = return_state;
  if (current_state == idle) {
    throw ice_t(pos, __FILE__, __LINE__);
  }
  return_state = idle;
  return current_state;
}

void lexer_t::push_state(state_t return_state_) {
  if (return_state != idle) {
    throw ice_t(pos, __FILE__, __LINE__);
  }
  return_state = return_state_;
}

// void lexer_t::emit_token(std::shared_ptr<token_t> token) {
//   // invoke a callback here for tree construction?
//   tokens.push_back(*token);
// }

void lexer_t::reset_temporary_buffer() {
  temporary_buffer.str("");
  temporary_buffer.clear();
}

void lexer_t::reset_tag_name_buffer() {
  tag_name_buffer.str("");
  tag_name_buffer.clear();
}

bool lexer_t::is_consuming_part_of_attribute() {
  switch (return_state) {
    case attribute_value_double_quoted:
    case attribute_value_single_quoted:
    case attribute_value_unquoted: {
      return true;
    }
    default: {
      return false;
    }
  }
}

void lexer_t::flush_consumed_as_character_reference() {
  if (is_consuming_part_of_attribute()) {
    attribute_value_buffer << temporary_buffer.str();
  } else {
    auto text = temporary_buffer.str().c_str();
    emit_token(character_t(anchor_pos, text));
  }
}

void lexer_t::emit_parse_error(const std::string &error) {
  this->on_parse_error(lexer_error_t(this, error.c_str()));
}

void lexer_t::set_current_tag_self_closing(bool current_tag_self_closing_) {
  current_tag_self_closing = current_tag_self_closing_;
}

void lexer_t::emit_token(const comment_t &token) {
  this->on_comment(token);
}

void lexer_t::emit_token(const doctype_t &token) {
  this->on_doctype(token);
}

void lexer_t::emit_token(const character_t &token) {
  this->on_character(token);
}

void lexer_t::emit_token(const eof_t &token) {
  this->on_eof(token);
}

void lexer_t::emit_token(const tag_t &token) {
  if (token.get_kind() == token_t::START_TAG) {
    last_start_tag_name = token.get_tag_name();
  }
  this->on_tag(token);
}

bool lexer_t::is_appropriate_end_tag() {
  return (
    temp_tag_token->get_tag_name() == last_start_tag_name &&
    temp_tag_token->get_kind() == token_t::END_TAG
  );
}

void lexer_t::lex() {
  bool go = true;
  do {
    char c = peek();
    // std::cout << "DEBUG: " << get_state_name(state) << " " << (!isspace(c) ? c : ' ') << std::endl;

    switch (state) {
      case idle: {
        throw ice_t(pos, __FILE__, __LINE__);
      }
      case data: {
        switch (c) {
          case '&': {
            set_anchor();
            push_state(data);
            pop();
            state = character_reference;
            break;
          }
          case '<': {
            set_anchor();
            pop();
            state = tag_open;
            break;
          }
          case '\0': {
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            emit_token(character_t(pos, c));
            pop();
            break;
          }
        }
        break;
      }
      case rcdata: {
        switch (c) {
          case '&': {
            push_state(rcdata);
            pop();
            state = character_reference;
            break;
          }
          case '<': {
            pop();
            state = rcdata_less_than_sign;
            break;
          }
          case '\0': {
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case rawtext: {
        switch (c) {
          case '<': {
            pop();
            state = rawtext_less_than_sign;
            break;
          }
          case '\0': {
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            emit_token(character_t(pos, c));
            pop();
            break;
          }
        }
        break;
      }
      case script_data: {
        switch (c) {
          case '<': {
            pop();
            state = script_data_less_than_sign;
            break;
          }
          case '\0': {
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            emit_token(character_t(pos, c));
            pop();
            break;
          }
        }
        break;
      }
      case plaintext: {
        if (c == '\0') {
          emit_token(eof_t(pos));
          go = false;
        } else {
          emit_token(character_t(pos, c));
          pop();
        }
        break;
      }
      case tag_open: {
        switch (c) {
          case '!': {
            pop();
            state = markup_declaration_open;
            break;
          }
          case '/': {
            pop();
            state = end_tag_open;
            break;
          }
          case '?': {
            emit_parse_error("unexpected-question-mark-instead-of-tag-name");
            state = bogus_comment;
            break;
          }
          case '\0': {
            emit_parse_error("eof-before-tag-name");
            std::string less_than_text(1, '<');
            emit_token(character_t(pos, c));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isalpha(c)) {
              state = tag_name;
              reset_tag_name_buffer();
              temp_tag_token = std::make_shared<tag_t>(anchor_pos);
            } else {
              emit_parse_error("invalid-first-character-of-tag-name");
              emit_token(character_t(pos, '<'));
            }
            break;
          }
        }
        break;
      }
      case end_tag_open: {
        switch (c) {
          case '>': {
            emit_parse_error("missing-end-tag-name");
            state = data;
            pop();
            break;
          }
          case '\0': {
            std::string char_text(1, '<');
            emit_token(character_t(pos, c));
            std::string char_solidus(1, '\\');
            emit_token(character_t(pos, c));
            emit_token(eof_t(pos));
            emit_parse_error("eof-before-tag-name");
            break;
          }
          default: {
            if (isalpha(c)) {
              reset_tag_name_buffer();
              temp_tag_token = std::make_shared<tag_t>(anchor_pos, true);
              state = tag_name;
            } else {
              emit_parse_error("invalid-first-character-of-tag-name");
              state = bogus_comment;
            }
            break;
          }
        }
        break;
      }
      case tag_name: {
        switch (c) {
          case '/': {
            state = self_closing_start_tag;
            pop();
            break;
          }
          case '>': {
            pop();
            state = data;
            emit_token(*temp_tag_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(eof_t(pos));
            break;
          }
          default: {
            if (isspace(c)) {
              state = before_attribute_name;
              pop();
            } else {
              pop();
              if (isupper(c)) {
                temp_tag_token->append_tag_name(char(tolower(c)));
              } else {
                temp_tag_token->append_tag_name(c);
              }
            }
            break;
          }
        }
        break;
      }
      case rcdata_less_than_sign: {
        if (c == '/') {
          pop();
          reset_temporary_buffer();
          state = rcdata_end_tag_open;
        } else {
          emit_token(character_t(pos, '<'));
          state = rcdata;
        }
        break;
      }
      case rcdata_end_tag_open: {
        if (isalpha(c)) {
          temp_tag_token = std::make_shared<tag_t>(pos, true);
          state = rcdata_end_tag_name;
        } else {
          emit_token(character_t(pos, c));
          state = rcdata;
        }
        break;
      }
      case rcdata_end_tag_name: {
        switch (c) {
          case '/': {
            // If the current end tag token is an appropriate end tag token, then switch
            // to the self_closing_start_tag state. otherwise treat is as per the anything
            // else entry below
            if (is_appropriate_end_tag()) {
              pop();
              state = self_closing_start_tag;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = rcdata;
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token, then switch
            // to the data state. otherwise treat is as per the anything
            // else entry below
            if (is_appropriate_end_tag()) {
              pop();
              state = data;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = rcdata;
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token, then switch
              // to the before_attribute_name state. otherwise treat is as per the anything
              // else entry below
              if (is_appropriate_end_tag()) {
                pop();
                state = before_attribute_name;
              } else {
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
                state = rcdata;
              }
            } else if (isalpha(c)) {
              pop();
              if (isupper(c)) {
                temp_tag_token->append_tag_name(char(tolower(c)));
              } else {
                temp_tag_token->append_tag_name(c);
              }
              temporary_buffer << c;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = rcdata;
            }
            break;
          }
        }
        break;
      }
      case rawtext_less_than_sign: {
        if (c == '/') {
          pop();
          reset_temporary_buffer();
          state = rawtext_end_tag_open;
        } else {
          emit_token(character_t(pos, '<'));
          state = rawtext;
        }
        break;
      }
      case rawtext_end_tag_open: {
        if (isalpha(c)) {
          temp_tag_token = std::make_shared<tag_t>(pos, c);
          state = rawtext_end_tag_name;
        } else {
          emit_token(character_t(pos, '<'));
          emit_token(character_t(pos, '/'));
          state = rawtext;
        }
        break;
      }
      case rawtext_end_tag_name: {
        switch (c) {
          case '/': {
            // If the current end tag token is an appropriate end tag token,
            // then switch to the self_closing_start_tag state. Otherwise, treat
            // it as per the "anything else" entry below.
            if (is_appropriate_end_tag()) {
              pop();
              state = self_closing_start_tag;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = rawtext;
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token,
            // then switch to the data state. Otherwise, treat
            // it as per the "anything else" entry below.
            if (is_appropriate_end_tag()) {
              pop();
              state = data;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = rawtext;
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token,
              // then switch to the before_attribute_name state. Otherwise, treat
              // it as per the "anything else" entry below.
              if (is_appropriate_end_tag()) {
                pop();
                state = before_attribute_name;
              } else {
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
                state = rawtext;
              }
            } else if (isalpha(c)) {
              pop();
              if (isupper(c)) {
                temp_tag_token->append_tag_name(char(tolower(c)));
              } else {
                temp_tag_token->append_tag_name(c);
              }
              temporary_buffer << c;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = rawtext;
            }
            break;
          }
        }
        break;
      }
      case script_data_less_than_sign: {
        switch (c) {
          case '/': {
            pop();
            reset_temporary_buffer();
            state = script_data_end_tag_open;
            break;
          }
          case '!': {
            pop();
            state = script_data_escape_start;
            emit_token(character_t(pos, '<'));
            emit_token(character_t(pos, '!'));
            break;
          }
          default: {
            emit_token(character_t(pos, '<'));
            state = script_data;
          }
        }
        break;
      }
      case script_data_end_tag_open: {
        if (isalpha(c)) {
          pop();
          temp_tag_token = std::make_shared<tag_t>(pos, true);
          state = script_data_end_tag_name;
        } else {
          emit_token(character_t(pos, '<'));
          emit_token(character_t(pos, '/'));
        }
        break;
      }
      case script_data_end_tag_name: {
        switch (c) {
          case '/': {
            // If the current end tag token is an appropriate end tag token,
            // then switch to the self_closing_start_tag state. Otherwise, treat
            // it as per the "anything else" entry below.
            if (is_appropriate_end_tag()) {
              pop();
              state = self_closing_start_tag;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = script_data;
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token,
            // then switch to the data state and emit the current tag token. Otherwise,
            // treat it as per the "anything else" entry below.
            if (is_appropriate_end_tag()) {
              pop();
              state = data;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = script_data;
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token,
              // then switch to the before attribute name state. Otherwise, treat
              // it as per the "anything else" entry below.
              if (is_appropriate_end_tag()) {
                pop();
                state = before_attribute_name;
              } else {
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
                state = script_data;
              }
            } else if (isalpha(c)) {
              pop();
              if (isupper(c)) {
                temp_tag_token->append_tag_name(char(tolower(c)));
              } else {
                temp_tag_token->append_tag_name(c);
              }
              temporary_buffer << c;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = script_data;
            }
          }
        }
        break;
      }
      case script_data_escape_start: {
        if (c == '-') {
          pop();
          emit_token(character_t(pos, '-'));
          state = script_data_escape_start_dash;
        } else {
          state = script_data;
        }
        break;
      }
      case script_data_escape_start_dash: {
        if (c == '-') {
          pop();
          state = script_data_escaped_dash_dash;
          emit_token(character_t(pos, '-'));
        } else {
          state = script_data;
        }
        break;
      }
      case script_data_escaped: {
        switch (c) {
          case '-': {
            pop();
            state = script_data_escaped_dash;
            emit_token(character_t(pos, '-'));
            break;
          }
          case '<': {
            pop();
            state = script_data_escaped_less_than_sign;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-script-html-comment-like-text");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            emit_token(character_t(pos, c));
          }
        }
        break;
      }
      case script_data_escaped_dash: {
        switch (c) {
          case '-': {
            pop();
            state = script_data_escaped_dash_dash;
            emit_token(character_t(pos, '-'));
            break;
          }
          case '<': {
            pop();
            state = script_data_escaped_less_than_sign;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-script-html-comment-like-text");
            emit_token(eof_t(pos));
            break;
          }
          default: {
            pop();
            state = script_data_escaped;
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_escaped_dash_dash: {
        switch (c) {
          case '-': {
            pop();
            emit_token(character_t(pos, '-'));
            break;
          }
          case '<': {
            pop();
            emit_token(character_t(pos, '<'));
            break;
          }
          case '>': {
            pop();
            emit_token(character_t(pos, '>'));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-script-html-comment-like-text");
            emit_token(eof_t(pos));
            break;
          }
          default: {
            pop();
            state = script_data_escaped;
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_escaped_less_than_sign: {
        switch (c) {
          case '/': {
            pop();
            reset_temporary_buffer();
            state = script_data_escaped_end_tag_open;
            break;
          }
          default: {
            if (isalpha(c)) {
              reset_temporary_buffer();
              emit_token(character_t(pos, '<'));
              state = script_data_double_escape_start;
            } else {
              emit_token(character_t(pos, '<'));
              state = script_data_escaped;
            }
          }
        }
        break;
      }
      case script_data_escaped_end_tag_open: {
        if (isalpha(c)) {
          temp_tag_token = std::make_shared<tag_t>(pos, true);
          state = script_data_escaped_end_tag_name;
        } else {
          emit_token(character_t(pos, '<'));
          emit_token(character_t(pos, '/'));
          state = script_data_escaped;
        }
        break;
      }
      case script_data_escaped_end_tag_name: {
        switch (c) {
          case '/': {
            // If the current end tag token is an appropriate end tag token, then switch to
            // the self-closing start tag state. Otherwise, treat it as per the "anything else"
            // entry below.
            if (is_appropriate_end_tag()) {
              pop();
              state = self_closing_start_tag;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = script_data_escaped;
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token, then switch to
            // the data state and emit the current tag token. Otherwise, treat it as per the
            // "anything else" entry below.
            if (is_appropriate_end_tag()) {
              pop();
              state = data;
            } else {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = script_data_escaped;
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token, then switch
              // to the before attribute name state. Otherwise, treat it as per the "anything else"
              // entry below.
              if (is_appropriate_end_tag()) {
                pop();
                state = before_attribute_name;
              } else {
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
                state = script_data_escaped;
              }
            } else if (isalpha(c)) {
              if (isupper(c)) {
                temp_tag_token->append_tag_name(char(tolower(c)));
              } else {
                temp_tag_token->append_tag_name(c);
              }
              temporary_buffer << c;
            } else {
              // Emit a U+003C LESS-THAN SIGN character token, a U+002F SOLIDUS character token, and
              // a character token for each of the characters in the temporary buffer (in the order
              // they were added to the buffer). Reconsume in the script data escaped state.
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
              state = script_data_escaped;
            }
            break;
          }
        }
        break;
      }
      case script_data_double_escape_start: {
        switch (c) {
          case '/':
          case '>': {
            // If the temporary buffer is the string "script", then switch to the script data double
            // escaped state. Otherwise, switch to the script data escaped state. Emit the current
            // input character as a character token.
            auto str = temporary_buffer.str();
            if (str == "script") {
              pop();
              state = script_data_double_escaped;
            } else {
              pop();
              state = script_data_escaped;
              emit_token(character_t(pos, c));
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the temporary buffer is the string "script", then switch to the script data double
              // escaped state. Otherwise, switch to the script data escaped state. Emit the current
              // input character as a character token.
              auto str = temporary_buffer.str();
              if (str == "script") {
                pop();
                state = script_data_double_escaped;
              } else {
                pop();
                state = script_data_escaped;
                emit_token(character_t(pos, c));
              }
            } else if (isalpha(c)) {
              pop();
              if (isupper(c)) {
                emit_token(character_t(pos, c));
              } else {
                emit_token(character_t(pos, c));
              }
              temporary_buffer << c;
            } else {
              state = script_data_escaped;
            }
          }
        }
        break;
      }
      case script_data_double_escaped: {
        switch (c) {
          case '-': {
            pop();
            state = script_data_double_escaped_dash;
            emit_token(character_t(pos, c));
            break;
          }
          case '<': {
            pop();
            state = script_data_double_escaped_less_than_sign;
            emit_token(character_t(pos, c));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-script-html-comment-like-text");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_double_escaped_dash: {
        switch (c) {
          case '-': {
            pop();
            state = script_data_double_escaped_dash_dash;
            emit_token(character_t(pos, c));
            break;
          }
          case '<': {
            pop();
            emit_token(character_t(pos, c));
            state = script_data_double_escaped_less_than_sign;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-script-html-comment-like-text");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            state = script_data_double_escaped;
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_double_escaped_dash_dash: {
        switch (c) {
          case '-': {
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '<': {
            pop();
            state = script_data_double_escaped_less_than_sign;
            emit_token(character_t(pos, c));
            break;
          }
          case '>': {
            pop();
            state = script_data;
            emit_token(character_t(pos, c));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-script-html-comment-like-text");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            state = script_data_double_escaped;
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_double_escaped_less_than_sign: {
        if (c == '/') {
          pop();
          reset_temporary_buffer();
          state = script_data_double_escape_end;
          emit_token(character_t(pos, c));
        } else {
          state = script_data_double_escaped;
        }
        break;
      }
      case script_data_double_escape_end: {
        switch (c) {
          case '/':
          case '>': {
            auto str = temporary_buffer.str();
            pop();
            state = script_data_escaped;
            if (str != "script") {
              emit_token(character_t(pos, c));
            }
            break;
          }
          default: {
            if (isspace(c)) {
              auto str = temporary_buffer.str();
              pop();
              state = script_data_escaped;
              if (str != "script") {
                emit_token(character_t(pos, c));
              }
            } else if (isalpha(c)) {
              if (isupper(c)) {
                temporary_buffer << char(tolower(c));
              } else {
                temporary_buffer << c;
              }
              emit_token(character_t(pos, c));
            } else {
              state = script_data_double_escaped;
            }
          }
        }
        break;
      }
      case before_attribute_name: {
        switch (c) {
          case '/':
          case '>':
          case '\0': {
            state = after_attribute_name;
            break;
          }
          case '=': {
            emit_parse_error("unexpected-equals-sign-before-attribute-name");
            current_token_attributes.push_back(std::pair<std::string, std::string>({"=", ""}));
            state = attribute_name;
            pop();
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              temp_tag_token->start_new_attribute();
              state = attribute_name;
            }
            break;
          }
        }
        break;
      }
      case attribute_name: {
        switch (c) {
          case '/':
          case '>':
          case '\0': {
            state = after_attribute_name;
            break;
          }
          case '=': {
            pop();
            state = before_attribute_value;
            break;
          }
          case '"':
          case '\'':
          case '<': {
            emit_parse_error("unexpected-character-in-attribute-name");
            std::get<0>(current_token_attributes.back()) += c;
            pop();
            break;
          }
          default: {
            if (isspace(c)) {
              state = after_attribute_name;
            } else if (isalpha(c)) {
              pop();
              if (isupper(c)) {
                temp_tag_token->append_attribute_name(char(tolower(c)));
              } else {
                temp_tag_token->append_attribute_name(c);
              }
            } else {
              pop();
              temp_tag_token->append_attribute_name(c);
            }
          }
        }
        break;
      }
      case after_attribute_name: {
        switch (c) {
          case '/': {
            pop();
            state = self_closing_start_tag;
            break;
          }
          case '=': {
            pop();
            state = before_attribute_value;
            break;
          }
          case '>': {
            pop();
            state = data;
            emit_token(*temp_tag_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              temp_tag_token->start_new_attribute();
              state = attribute_name;
            }
            break;
          }
        }
        break;
      }
      case before_attribute_value: {
        switch (c) {
          case '"': {
            pop();
            state = attribute_value_double_quoted;
            break;
          }
          case '\'': {
            pop();
            state = attribute_value_single_quoted;
            break;
          }
          case '>': {
            emit_parse_error("missing-attribute-value");
            pop();
            emit_token(*temp_tag_token);
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              state = attribute_value_unquoted;
            }
          }
        }
        break;
      }
      case attribute_value_double_quoted: {
        switch (c) {
          case '"': {
            pop();
            state = after_attribute_value_quoted;
            break;
          }
          case '&': {
            push_state(attribute_value_double_quoted);
            pop();
            state = character_reference;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            go = false;
            break;
          }
          default: {
            temp_tag_token->append_attribute_value(c);
            pop();
            break;
          }
        }
        break;
      }
      case attribute_value_single_quoted: {
        switch (c) {
          case '\'': {
            pop();
            state = after_attribute_value_quoted;
            break;
          }
          case '&': {
            push_state(attribute_value_single_quoted);
            pop();
            state = character_reference;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            temp_tag_token->append_attribute_value(c);
            pop();
            break;
          }
        }
        break;
      }
      case attribute_value_unquoted: {
        switch (c) {
          case '&': {
            pop();
            push_state(attribute_value_unquoted);
            state = character_reference;
            break;
          }
          case '>': {
            pop();
            state = data;
            emit_token(*temp_tag_token);
            break;
          }
          case '"':
          case '\'':
          case '<':
          case '=':
          case '`': {
            pop();
            emit_parse_error("unexpected-character-in-unquoted-attribute-value");
            temp_tag_token->append_attribute_value(c);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_attribute_name;
            } else {
              pop();
              temp_tag_token->append_attribute_value(c);
            }
            break;
          }
        }
        break;
      }
      case after_attribute_value_quoted: {
        switch (c) {
          case '/': {
            pop();
            state = self_closing_start_tag;
            break;
          }
          case '>': {
            pop();
            state = data;
            emit_token(*temp_tag_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_attribute_name;
            } else {
              emit_parse_error("missing-whitespace-between-attributes");
              state = before_attribute_name;
            }
            break;
          }
        }
        break;
      }
      case self_closing_start_tag: {
        switch (c) {
          case '>': {
            pop();
            temp_tag_token->set_self_closing(true);
            emit_token(*temp_tag_token);
            state = data;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            emit_parse_error("unexpected-solidus-in-tag");
            state = before_attribute_name;
            break;
          }
        }
        break;
      }
      case bogus_comment: {
        switch (c) {
          case '>': {
            pop();
            state = data;
            emit_token(comment_t(pos));
            break;
          }
          case '\0': {
            emit_token(comment_t(pos));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            break;
          }
        }
        break;
      }
      case markup_declaration_open: {
        switch (c) {
          case '-': {
            const char *cursor_peek = cursor;
            pop();
            c = peek();
            if (c == '-') {
              pop();
              state = comment_start;
              break;
            }
            reset_cursor(cursor_peek);
            c = peek();
            state = bogus_comment;
            break;
          }
          case 'D':
          case 'd': {
            const char *cursor_peek = cursor;
            pop();
            c = peek();
            if (c == 'o' || c == 'O') {
              pop();
              c = peek();
              if (c == 'c' || c == 'C') {
                pop();
                c = peek();
                if (c == 't' || c == 'T') {
                  pop();
                  c = peek();
                  if (c == 'y' || c == 'Y') {
                    pop();
                    c = peek();
                    if (c == 'p' || c == 'P') {
                      pop();
                      c = peek();
                      if (c == 'e' || c == 'E') {
                        pop();
                        state = doctype;
                        break;
                      }
                    }
                  }
                }
              }
            }
            reset_cursor(cursor_peek);
            c = peek();
            state = bogus_comment;
            break;
          }
          case '[': {
            const char *cursor_peek = cursor;
            pop();
            c = peek();
            if (c == 'C') {
              pop();
              c = peek();
              if (c == 'D') {
                pop();
                c = peek();
                if (c == 'A') {
                  pop();
                  c = peek();
                  if (c == 'T') {
                    pop();
                    c = peek();
                    if (c == 'A') {
                      pop();
                      c = peek();
                      if (c == '[') {
                        if (stack_of_open_elements.size() > 1) {
                          emit_parse_error("cdata-in-html-content");
                          state = bogus_comment;
                        } else {
                          state = cdata_section;
                        }
                        break;
                      }
                    }
                  }
                }
              }
            }
            reset_cursor(cursor_peek);
            c = peek();
            state = bogus_comment;
            break;
          }
          default: {
            emit_parse_error("incorrectly-opened-comment");
            reset_temporary_buffer();
            state = bogus_comment;
            break;
          }
        }
        break;
      }
      case comment_start: {
        switch (c) {
          case '-': {
            pop();
            state = comment_start_dash;
            break;
          }
          case '>': {
            pop();
            emit_parse_error("abrupt-closing-of-empty-comment");
            state = data;
            emit_token(comment_t(pos));
            break;
          }
          default: {
            state = comment;
            break;
          }
        }
        break;
      }
      case comment_start_dash: {
        switch (c) {
          case '-': {
            pop();
            state = comment_end;
            break;
          }
          case '>': {
            pop();
            emit_parse_error("abrupt-closing-of-empty-comment");
            state = data;
            emit_token(comment_t(pos));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(comment_t(pos));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            state = comment;
            break;
          }
        }
        break;
      }
      case comment: {
        switch (c) {
          case '<': {
            pop();
            state = comment_less_than_sign;
            break;
          }
          case '-': {
            pop();
            state = comment_end_dash;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(comment_t(pos));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            break;
          }
        }
        break;
      }
      case comment_less_than_sign: {
        switch (c) {
          case '!': {
            pop();
            state = comment_less_than_sign_bang;
            break;
          }
          case '<': {
            pop();
            break;
          }
          default: {
            state = comment;
            break;
          }
        }
        break;
      }
      case comment_less_than_sign_bang: {
        if (c == '-') {
          pop();
          state = comment_less_than_sign_bang_dash;
        } else {
          state = comment;
        }
        break;
      }
      case comment_less_than_sign_bang_dash: {
        if (c == '-') {
          pop();
          state = comment_less_than_sign_bang_dash_dash;
        } else {
          state = comment_end_dash;
        }
        break;
      }
      case comment_less_than_sign_bang_dash_dash: {
        switch (c) {
          case '>':
          case '\0': {
            state = comment_end;
            break;
          }
          default: {
            emit_parse_error("nested-comment");
            state = comment_end;
            break;
          }
        }
        break;
      }
      case comment_end_dash: {
        switch (c) {
          case '-': {
            pop();
            state = comment_end;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(comment_t(pos));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            state = comment;
            break;
          }
        }
        break;
      }
      case comment_end: {
        switch (c) {
          case '>': {
            pop();
            state = data;
            emit_token(comment_t(pos));
            break;
          }
          case '!': {
            pop();
            state = comment_end_bang;
            break;
          }
          case '-': {
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(comment_t(pos));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            state = comment;
            break;
          }
        }
        break;
      }
      case comment_end_bang: {
        switch (c) {
          case '-': {
            pop();
            state = comment_end_dash;
            break;
          }
          case '>': {
            pop();
            emit_parse_error("incorrectly-closed-comment");
            state = data;
            emit_token(comment_t(pos));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(comment_t(pos));
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            state = comment;
            break;
          }
        }
        break;
      }
      case doctype: {
        switch (c) {
          case '>': {
            state = before_doctype_name;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_doctype_name;
            } else {
              emit_parse_error("missing-whitespace-before-doctype-name");
              state = before_doctype_name;
            }
            break;
          }
        }
        break;
      }
      case before_doctype_name: {
        switch (c) {
          case '>': {
            pop();
            emit_parse_error("unexpected-null-character");
            emit_token(*temp_doctype_token);
            state = data;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else if (isalpha(c)) {
              pop();
              temp_doctype_token = std::make_shared<doctype_t>(pos);
              if (isupper(c)) {
                temp_doctype_token->append_doctype_name(char(tolower(c)));
              } else {
                temp_doctype_token->append_doctype_name(c);
              }
              state = doctype_name;
            } else {
              pop();
              temp_doctype_token = std::make_shared<doctype_t>(pos);
              temp_doctype_token->append_doctype_name(c);
              state = doctype_name;
            }
            break;
          }
        }
        break;
      }
      case doctype_name: {
        switch (c) {
          case '>': {
            pop();
            emit_token(*temp_doctype_token);
            state = data;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = after_doctype_name;
            } else if (isalpha(c)) {
              pop();
              if (isupper(c)) {
                temp_doctype_token->append_doctype_name(char(tolower(c)));
              } else {
                temp_doctype_token->append_doctype_name(c);
              }
            } else {
              pop();
              temp_doctype_token->append_doctype_name(c);
            }
            break;
          }
        }
        break;
      }
      case after_doctype_name: {
        switch (c) {
          case '>': {
            pop();
            emit_token(*temp_doctype_token);
            state = data;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              std::string six_chars;
              const char *current_cursor = cursor;;
              for (int i = 0; i < 6; ++i) {
                if (isupper(c)) {
                  six_chars += char(tolower(c));
                } else {
                  six_chars += c;
                }
                pop();
                c = peek();
                if (c == '\0') {
                  break;
                }
              }
              if (six_chars == "public") {
                state = after_doctype_public_keyword;
              } else if (six_chars == "system") {
                state = after_doctype_system_keyword;
              } else {
                emit_parse_error("invalid-character-sequence-after-doctype-name");
                reset_cursor(current_cursor);
                state = bogus_doctype;
              }
            }
            break;
          }
        }
        break;
      }
      case after_doctype_public_keyword: {
        switch (c) {
          case '"': {
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            temp_doctype_token->set_public_identifier("");
            pop();
            state = doctype_public_identifier_double_quoted;
            break;
          }
          case '\'': {
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            temp_doctype_token->set_public_identifier("");
            state = doctype_public_identifier_single_quoted;
            pop();
            break;
          }
          case '>': {
            emit_parse_error("missing-doctype-public-identifier");
            pop();
            state = data;
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_doctype_public_identifier;
            } else {
              emit_parse_error("missing-quote-before-doctype-public-identifier");
              temp_doctype_token->set_force_quirks(true);
              state = bogus_doctype;
            }
            break;
          }
        }
        break;
      }
      case before_doctype_public_identifier: {
        switch (c) {
          case '"': {
            pop();
            temp_doctype_token->set_public_identifier("");
            state = doctype_public_identifier_double_quoted;
            break;
          }
          case '\'': {
            pop();
            temp_doctype_token->set_public_identifier("");
            state = doctype_public_identifier_single_quoted;
            break;
          }
          case '>': {
            pop();
            emit_parse_error("missing-doctype-public-identifier");
            temp_doctype_token->set_force_quirks(true);
            state = data;
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("missing-quote-before-doctype-public-identifier");
              temp_doctype_token->set_force_quirks(true);
              state = bogus_doctype;
            }
            break;
          }
        }
        break;
      }
      case doctype_public_identifier_double_quoted: {
        switch (c) {
          case '"': {
            pop();
            state = after_doctype_public_identifier;
            break;
          }
          case '>': {
            emit_parse_error("abrupt-doctype-public-identifier");
            pop();
            temp_doctype_token->set_force_quirks(true);
            state = data;
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            temp_doctype_token->append_public_identifier(c);
            pop();
            break;
          }
        }
        break;
      }
      case doctype_public_identifier_single_quoted: {
        switch (c) {
          case '\'': {
            pop();
            state = after_doctype_public_identifier;
            break;
          }
          case '>': {
            emit_parse_error("abrupt-doctype-public-identifier");
            temp_doctype_token->set_force_quirks(true);
            state = data;
            emit_token(*temp_doctype_token);
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            temp_doctype_token->append_public_identifier(c);
            pop();
            break;
          }
        }
        break;
      }
      case after_doctype_public_identifier: {
        switch (c) {
          case '>': {
            pop();
            state = data;
            emit_token(*temp_doctype_token);
            break;
          }
          case '"': {
            emit_parse_error("missing-whitespace-between-doctype-public-and-system-identifiers");
            temp_doctype_token->set_system_identifier("");
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            emit_parse_error("missing-whitespace-between-doctype-public-and-system-identifiers");
            pop();
            temp_doctype_token->set_system_identifier("");
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = between_doctype_public_and_system_identifiers;
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              temp_doctype_token->set_force_quirks(true);
              state = bogus_doctype;
            }
            break;
          }
        }
        break;
      }
      case between_doctype_public_and_system_identifiers: {
        switch (c) {
          case '>': {
            pop();
            state = data;
            emit_token(*temp_doctype_token);
            break;
          }
          case '"': {
            temp_doctype_token->set_system_identifier("");
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            temp_doctype_token->set_system_identifier("");
            pop();
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              temp_doctype_token->set_force_quirks(true);
              state = bogus_doctype;
            }
            break;
          }
        }
        break;
      }
      case after_doctype_system_keyword: {
        switch (c) {
          case '>': {
            emit_parse_error("missing-doctype-system-identifier");
            temp_doctype_token->set_force_quirks(true);
            pop();
            state = data;
            emit_token(*temp_doctype_token);
            break;
          }
          case '"': {
            temp_doctype_token->set_system_identifier("");
            emit_parse_error("missing-whitespace-after-doctype-system-keyword");
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            temp_doctype_token->set_system_identifier("");
            emit_parse_error("missing-whitespace-after-doctype-system-keyword");
            pop();
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_doctype_system_identifier;
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              temp_doctype_token->set_force_quirks(true);
              state = bogus_doctype;
            }
            break;
          }
        }
        break;
      }
      case before_doctype_system_identifier: {
        switch (c) {
          case '>': {
            emit_parse_error("missing-doctype-system-identifier");
            temp_doctype_token->set_force_quirks(true);
            pop();
            state = data;
            emit_token(*temp_doctype_token);
            break;
          }
          case '"': {
            temp_doctype_token->set_system_identifier("");
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            temp_doctype_token->set_system_identifier("");
            pop();
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              temp_doctype_token->set_force_quirks(true);
              state = bogus_doctype;
            }
            break;
          }
        }
        break;
      }
      case doctype_system_identifier_double_quoted: {
        switch (c) {
          case '"': {
            pop();
            state = after_doctype_system_identifier;
            break;
          }
          case '>': {
            emit_parse_error("abrupt-doctype-system-identifier");
            temp_doctype_token->set_force_quirks(true);
            state = data;
            emit_token(*temp_doctype_token);
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            temp_doctype_token->append_system_identifier(c);
            pop();
            break;
          }
        }
        break;
      }
      case doctype_system_identifier_single_quoted: {
        switch (c) {
          case '\'': {
            pop();
            state = after_doctype_system_identifier;
            break;
          }
          case '>': {
            emit_parse_error("abrupt-doctype-system-identifier");
            temp_doctype_token->set_force_quirks(true);
            state = data;
            emit_token(*temp_doctype_token);
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            temp_doctype_token->append_system_identifier(c);
            pop();
            break;
          }
        }
        break;
      }
      case after_doctype_system_identifier: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            temp_doctype_token->set_force_quirks(true);
            emit_parse_error("eof-in-doctype");
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("unexpected-character-after-doctype-system-identifier");
              state = bogus_doctype;
            }
          }
        }
        break;
      }
      case bogus_doctype: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            emit_token(*temp_doctype_token);
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            pop();
            break;
          }
        }
        break;
      }
      case cdata_section: {
        switch (c) {
          case ']': {
            pop();
            state = cdata_section_bracket;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-cdata");
            emit_token(eof_t(pos));
            go = false;
            break;
          }
          default: {
            emit_token(character_t(pos, c));
            pop();
            break;
          }
        }
        break;
      }
      case cdata_section_bracket: {
        if (c == ']') {
          pop();
          state = cdata_section_end;
        } else {
          emit_token(character_t(pos, c));
          state = cdata_section;
        }
        break;
      }
      case cdata_section_end: {
        switch (c) {
          case ']': {
            emit_token(character_t(pos, c));
            pop();
            break;
          }
          case '>': {
            pop();
            state = data;
            break;
          }
          default: {
            emit_token(character_t(pos, c));
            emit_token(character_t(pos, c));
            state = cdata_section;
            break;
          }
        }
        break;
      }
      case character_reference: {
        // Set the temporary buffer to the empty string. Append a U+0026 AMPERSAND (&)
        // character to the temporary buffer.
        reset_temporary_buffer();
        temporary_buffer << '&';
        if (isalnum(c)) {
          // Reconsume in the named character reference state.
          state = named_character_reference;
        } else if (c == '#') {
          // Append the current input character to the temporary buffer. Switch to
          // the numeric character reference state.
          temporary_buffer << c;
          pop();
          state = numeric_character_reference;
        } else {
          // Flush code points consumed as a character reference. Reconsume in the
          // return state.
          flush_consumed_as_character_reference();
          state = pop_state();
        }
        break;
      }
      case named_character_reference: {
        // TODO - Generate a deterministric state table for properly handling
        // named character references.
        do {
          if (!isspace(c) && c != '\0') {
            // std::cout << "DEBUG named_character_reference " << c << std::endl;
            temporary_buffer << c;
            if (c == ';') {
              pop();
              c = peek();
              break;
            } else {
              pop();
              c = peek();
            }
          } else {
            break;
          }
        } while (true);

        auto codepoints = lookup_characters(temporary_buffer.str());
        if (codepoints.size()) {
          // std::cout << "DEBUG: named_character_reference 'found match " << temporary_buffer.str() << "'" << std::endl;
          reset_temporary_buffer();
          // for (const auto &point: codepoints) {
          //   std::cout << "DEBUG: named_character_reference 'codepoint " << point << "'" << std::endl;
          // }
          temporary_buffer << convert_codepoints_to_utf8(codepoints);
          flush_consumed_as_character_reference();
          state = pop_state();
        } else {
          // std::cout << "DEBUG: named_character_reference 'no match " << temporary_buffer.str() << "'" << std::endl;
          state = ambiguous_ampersand;
          flush_consumed_as_character_reference();
        }
        break;
      }
      case ambiguous_ampersand: {
        if (isalnum(c)) {
          // If the character reference was consumed as part of an attribute, then
          // append the current input character to the current attribute's value. Otherwise,
          // emit the current input character as a character token.
          if (is_consuming_part_of_attribute()) {
            attribute_value_buffer << c;
          } else {
            emit_token(character_t(pos, c));
          }
        } else if (c == ';') {
          // This is an unknown-named-character-reference parse error. Reconsume in the
          // return state.
          emit_parse_error("unknown-named-character-reference-parse-error");
          state = pop_state();
        } else {
          // Reconsume in the return state
          state = pop_state();
        }
        break;
      }
      case numeric_character_reference: {
        // TODO set character reference code to zero
        switch (c) {
          case 'x':
          case 'X': {
            temporary_buffer << c;
            state = hexadecimal_character_reference_start;
            pop();
            break;
          }
          default: {
            state = decimal_character_reference_start;
            break;
          }
        }
        break;
      }
      case hexadecimal_character_reference_start: {
        if (isxdigit(c)) {
          state = hexadecimal_character_reference;
        } else {
          emit_parse_error("absense-of-digits-in-numeric-character-reference");
          flush_consumed_as_character_reference();
          state = pop_state();
        }
        break;
      }
      case decimal_character_reference_start: {
        if (isdigit(c)) {
          state = decimal_character_reference;
        } else {
          emit_parse_error("absense-of-digits-in-numeric-character-reference");
          flush_consumed_as_character_reference();
          state = pop_state();
        }
        break;
      }
      case hexadecimal_character_reference: {
        if (isdigit(c)) {
          // TODO
          // Multiply the character reference code by 16. Add a numeric version of
          // the current input character (subtract 0x0030 from the character's code
          // point) to the character reference code.
        } else if (isxdigit(c)) {
          if (isupper(c)) {
            // TODO
            // Multiply the character reference code by 16. Add a numeric version
            // of the current input character as a hexadecimal digit (subtract
            // 0x0037 from the character's code point) to the character reference code.
          } else {
            // TODO
            // Multiply the character reference code by 16. Add a numeric version of
            // the current input character as a hexadecimal digit (subtract 0x0057
            // from the character's code point) to the character reference code.
          }
        } else if (c == ';') {
          pop();
          state = numeric_character_reference_end;
        } else {
          emit_parse_error("missing-semicolon-after-character-reference");
          state = numeric_character_reference_end;
        }
        break;
      }
      case decimal_character_reference: {
        switch (c) {
          case ';': {
            pop();
            state = numeric_character_reference_end;
            break;
          }
          default: {
            if (isdigit(c)) {
              // TODO
              // Multiply the character reference code by 10. Add a numeric version of
              // the current input character (subtract 0x0030 from the character's code
              // point) to the character reference code.
            } else {
              emit_parse_error("missing-semicolon-after-character-reference");
              state = numeric_character_reference_end;
            }
          }
        }
        break;
      }
      case numeric_character_reference_end: {
        // TODO: Check the character reference code:

        // If the number is 0x00, then this is a null-character-reference parse error.
        // Set the character reference code to 0xFFFD.

        // If the number is greater than 0x10FFFF, then this is a
        // character-reference-outside-unicode-range parse error.
        // Set the character reference code to 0xFFFD.

        // If the number is a surrogate, then this is a surrogate-character-reference
        // parse error. Set the character reference code to 0xFFFD.

        // If the number is a noncharacter, then this is a noncharacter-character-reference
        // parse error.

        // If the number is 0x0D, or a control that's not ASCII whitespace, then this is a control-character-reference parse error. If the number is one of the numbers in the first column of the following table, then find the row with that number in the first column, and set the character reference code to the number in the second column of that row.
        reset_temporary_buffer();
        temporary_buffer << "?";
        flush_consumed_as_character_reference();
        state = pop_state();
        break;
      }
    }
  } while (go);
}

}   // yourcss
