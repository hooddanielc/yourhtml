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

void lexer_t::emit_token(std::shared_ptr<token_t> token) {
  // invoke a callback here for tree construction?
  tokens.push_back(token);
}

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
    auto text = temporary_buffer.str();
    emit_token(token_t::make(anchor_pos, token_t::CHARACTER, std::move(text)));
  }
}

void lexer_t::emit_parse_error(const std::string &error) {
  throw lexer_error_t(this, error.c_str());
}

void lexer_t::set_current_tag_self_closing(bool current_tag_self_closing_) {
  current_tag_self_closing = current_tag_self_closing_;
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
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            std::string char_text(1, c);
            emit_token(token_t::make(pos, token_t::CHARACTER, std::move(char_text)));
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
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, c)));
            pop();
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
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, c)));
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
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, c)));
            pop();
            break;
          }
        }
        break;
      }
      case plaintext: {
        if (c == '\0') {
          emit_token(token_t::make(pos, token_t::END_OF_FILE));
          go = false;
        } else {
          emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, c)));
          pop();
        }
        break;
      }
      case tag_open: {
        switch (c) {
          case '!': {
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
            emit_token(token_t::make(pos, token_t::CHARACTER, std::move(less_than_text)));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isalpha(c)) {
              state = tag_name;
              reset_tag_name_buffer();
              current_token = token_t::make(anchor_pos, token_t::START_TAG);
            } else {
              emit_parse_error("invalid-first-character-of-tag-name");
              std::string less_than_text(1, '<');
              emit_token(token_t::make(pos, token_t::CHARACTER, std::move(less_than_text)));
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
            emit_token(token_t::make(pos, token_t::CHARACTER, std::move(char_text)));
            std::string char_solidus(1, '\\');
            emit_token(token_t::make(pos, token_t::CHARACTER, std::move(char_solidus)));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            emit_parse_error("eof-before-tag-name");
            break;
          }
          default: {
            if (isalpha(c)) {
              reset_tag_name_buffer();
              current_token = token_t::make(anchor_pos, token_t::END_TAG);
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
            emit_token(stack_of_open_elements.back());
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            std::string replacement_text("\uFFFD");
            emit_token(token_t::make(pos, token_t::CHARACTER, std::move(replacement_text)));
            break;
          }
          default: {
            if (isspace(c)) {
              state = before_attribute_name;
              pop();
            } else {
              if (isupper(c)) {
                tag_name_buffer << tolower(c);
              } else {
                tag_name_buffer << c;
              }
            }
            break;
          }
        }
        break;
      }
      case rcdata_less_than_sign: {
        if (c == '/') {
          reset_temporary_buffer();
          pop();
          state = rcdata_end_tag_open;
        } else {
          emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, '<')));
          state = rcdata;
        }
        break;
      }
      case rcdata_end_tag_open: {
        if (isalpha(c)) {
          current_token = token_t::make(pos, token_t::END_TAG);
          state = rcdata_end_tag_name;
        } else {
          emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, '<')));
          state = rcdata;
        }
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
              auto current_char_text = std::string(1, c);
              current_token_attributes.push_back(std::pair<std::string, std::string>({"", ""}));
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
              if (isupper(c)) {
                std::get<0>(current_token_attributes.back()) += char(tolower(c));
              } else {
                std::get<0>(current_token_attributes.back()) += c;
              }
              pop();
            } else {
              std::get<0>(current_token_attributes.back()) += c;
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
            pop_anchor();
            emit_token(current_token);
            current_token = nullptr;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              current_token_attributes.push_back(std::pair<std::string, std::string>({"", ""}));
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
            emit_token(current_token);
            current_token = nullptr;
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
            std::get<1>(current_token_attributes.back()) += c;
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
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            std::get<1>(current_token_attributes.back()) += c;
            pop();
            break;
          }
        }
        break;
      }
      case attribute_value_unquoted: {
        switch (c) {
          case '&': {
            push_state(attribute_value_unquoted);
            pop();
            state = character_reference;
            break;
          }
          case '>': {
            pop();
            state = data;
            emit_token(current_token);
            break;
          }
          case '"':
          case '\'':
          case '<':
          case '=':
          case '`': {
            emit_parse_error("unexpected-character-in-unquoted-attribute-value");
            std::get<1>(current_token_attributes.back()) += c;
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              state = before_attribute_name;
            } else {
              std::get<1>(current_token_attributes.back()) += c;
              pop();
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
            emit_token(current_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            set_current_tag_self_closing(true);
            emit_token(current_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-tag");
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            state = data;
            emit_token(token_t::make(pos, token_t::COMMENT));
            break;
          }
          case '\0': {
            emit_token(token_t::make(pos, token_t::COMMENT));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(token_t::make(pos, token_t::COMMENT));
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
            emit_token(token_t::make(pos, token_t::COMMENT));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(token_t::make(pos, token_t::COMMENT));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(token_t::make(pos, token_t::COMMENT));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(token_t::make(pos, token_t::COMMENT));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(token_t::make(pos, token_t::COMMENT));
            break;
          }
          case '!': {
            state = comment_end_bang;
            pop();
            break;
          }
          case '-': {
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(token_t::make(pos, token_t::COMMENT));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(token_t::make(pos, token_t::COMMENT));
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-comment");
            emit_token(token_t::make(pos, token_t::COMMENT));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            current_token = token_t::make(pos, token_t::DOCTYPE);
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
            emit_token(token_t::make(pos, token_t::DOCTYPE));
            state = data;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            emit_token(token_t::make(pos, token_t::DOCTYPE));
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else if (isalpha(c)) {
              current_token = token_t::make(pos, token_t::DOCTYPE);
              if (isupper(c)) {
                current_token->append_name(std::string(1, char(tolower(c))));
              } else {
                current_token->append_name(std::string(1, c));
              }
            } else {
              current_token = token_t::make(pos, token_t::DOCTYPE);
              current_token->append_name(std::string(1, c));
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
            emit_token(current_token);
            state = data;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isalpha(c)) {
              if (isupper(c)) {
                current_token->append_name(std::string(1, char(tolower(c))));
              } else {
                current_token->append_name(std::string(1, c));
              }
            } else {
              current_token->append_name(std::string(1, c));
            }
            break;
          }
        }
        break;
      }
      case after_doctype_name: {
        switch (c) {
          case '>': {
            break;
          }
          case '\0': {
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
                pop();
              } else if (six_chars == "system") {
                state = after_doctype_system_keyword;
                pop();
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
            // TODO set doctypes token public identifier to empty string
            pop();
            state = doctype_public_identifier_double_quoted;
            break;
          }
          case '\'': {
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            // TODO set doctypes token public identifier to empty string
            state = doctype_public_identifier_single_quoted;
            pop();
            break;
          }
          case '>': {
            emit_parse_error("missing-doctype-public-identifier");
            // TODO set doctypes token's force quirks mode flag to on
            pop();
            state = data;
            emit_token(current_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_doctype_public_identifier;
            } else {
              emit_parse_error("missing-quote-before-doctype-public-identifier");
              // TODO set doctypes token's force quirks mode flag to on
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
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            // TODO set doctypes token public identifier to empty string
            pop();
            state = doctype_public_identifier_double_quoted;
            break;
          }
          case '\'': {
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            // TODO set doctypes token public identifier to empty string
            state = doctype_public_identifier_single_quoted;
            pop();
            break;
          }
          case '>': {
            emit_parse_error("missing-doctype-public-identifier");
            // TODO set doctypes token's force quirks mode flag to on
            pop();
            state = data;
            emit_token(current_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("missing-quote-before-doctype-public-identifier");
              // TODO set doctypes token's force quirks mode flag to on
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
            // todo set force-quirks mod flag to on
            state = data;
            emit_token(current_token);
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            // TODO append the current input character to the current doctype tokens public
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
            // todo set doctypes force-quirks flag to one
            state = data;
            emit_token(current_token);
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            // TODO append the current input character to the current DOCTYPE
            // tokens public identifier
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
            emit_token(current_token);
            break;
          }
          case '"': {
            emit_parse_error("missing-whitespace-between-doctype-public-and-system-identifiers");
            // TODO set doctypes token's system identifier to the empty string (not missing)
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            emit_parse_error("missing-whitespace-between-doctype-public-and-system-identifiers");
            pop();
            // TODO set doctypes token's system identifier to the empty string (not missing)
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = between_doctype_public_and_system_identifiers;
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              // TODO set doctypes token force-quirks mode flag to on
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
            emit_token(current_token);
            break;
          }
          case '"': {
            // TODO - set doctypes token's system identifier to the empty string (not missing)
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            // TODO - set doctypes token's system identifier to the empty string (not missing)
            pop();
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              // TODO set doctypes token force-quirks mode flag to on
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
            // TODO - set doctypes token's force-quirks mode flag to on
            pop();
            state = data;
            emit_token(current_token);
            break;
          }
          case '"': {
            // TODO - set doctypes token's system identifier to the empty string (not missing)
            emit_parse_error("missing-whitespace-after-doctype-system-keyword");
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            // TODO - set doctypes token's system identifier to the empty string (not missing)
            emit_parse_error("missing-whitespace-after-doctype-system-keyword");
            pop();
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
              state = before_doctype_system_identifier;
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              // TODO set doctypes token force-quirks mode flag to on
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
            // TODO - set doctypes token's force-quirks mode flag to on
            pop();
            state = data;
            emit_token(current_token);
            break;
          }
          case '"': {
            // TODO - set doctypes token's system identifier to the empty string (not missing)
            pop();
            state = doctype_system_identifier_double_quoted;
            break;
          }
          case '\'': {
            // TODO - set doctypes token's system identifier to the empty string (not missing)
            pop();
            state = doctype_system_identifier_single_quoted;
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              emit_parse_error("missing-quote-before-doctype-system-identifier");
              // TODO set doctypes token force-quirks mode flag to on
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
            // TODO set doctypes force-quirks mode flag to on
            state = data;
            emit_token(current_token);
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            // TODO append current token to the token's system identifier
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
            // TODO set doctypes force-quirks mode flag to on
            state = data;
            emit_token(current_token);
            pop();
            break;
          }
          case '\0': {
            emit_parse_error("eof-in-doctype");
            // TODO set doctypes token's force quirks mode flag to on
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            // TODO append current token to the token's system identifier
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
            emit_token(current_token);
            break;
          }
          case '\0': {
            // TODO set force quirks mode flag to on
            emit_parse_error("eof-in-doctype");
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(current_token);
            break;
          }
          case '\0': {
            emit_token(current_token);
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
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
            emit_token(token_t::make(pos, token_t::END_OF_FILE));
            go = false;
            break;
          }
          default: {
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, c)));
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
          emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, ']')));
          state = cdata_section;
        }
        break;
      }
      case cdata_section_end: {
        switch (c) {
          case ']': {
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, ']')));
            pop();
            break;
          }
          case '>': {
            pop();
            state = data;
            break;
          }
          default: {
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, ']')));
            emit_token(token_t::make(pos, token_t::CHARACTER, std::string(1, ']')));
            state = cdata_section;
            break;
          }
        }
        break;
      }
      case character_reference: {
        // Set the temporary buffer to the empty string. Append a U+0026 AMPERSAND (&)
        // character to the temporary buffer.
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
          pop_anchor();
          state = pop_state();
        }
        break;
      }
      case named_character_reference: {
        // Consume the maximum number of characters possible, with the consumed characters
        // matching one of the identifiers in the first column of the named character references
        // table (in a case-sensitive manner). Append each character to the temporary buffer when
        // it's consumed.
        bool found_match = false;
        std::string text;
        while (!isspace(c) && c != ';' && !found_match) {
          text = lookup_characters(temporary_buffer.str());
          if (!text.empty()) {
            temporary_buffer << c;
            found_match = true;
          }
          temporary_buffer << c;
          pop();
          c = peek();
        }
        if (!found_match) {
          text = lookup_characters(temporary_buffer.str());
          if (c == ';') {
            pop();
            c = peek();
          }
        }
        if (!text.empty()) {
          // found a match
          if (c != ';' && is_consuming_part_of_attribute() && (c == '=' || isalpha(c))) {
            // for historical reasons, flush cod points consumed as character reference
            flush_consumed_as_character_reference();
            state = pop_state();
          } else {
            if (c != ';') {
              emit_parse_error("missing-semicolon-after-character-reference");
            }
            flush_consumed_as_character_reference();
          }
        } else {
          flush_consumed_as_character_reference();
          state = ambiguous_ampersand;
          pop();
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
            std::string text(1, c);
            emit_token(token_t::make(pos, token_t::CHARACTER, std::move(text)));
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
  return tokens;
}

}   // yourcss
