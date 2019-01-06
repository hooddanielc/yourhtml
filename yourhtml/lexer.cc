#include "lexer.h"

namespace yourhtml {

lexer_t::lexer_error_t::lexer_error_t(const lexer_t *lexer, const char *msg):
  error_t(lexer->pos),
  type(msg) {
  get_strm() << msg;
}

lexer_t::lexer_error_t::~lexer_error_t() = default;

std::string lexer_t::lexer_error_t::get_type() const {
  return type;
}

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
   lexer_t::lexer_t(next_cursor_, std::strlen(next_cursor_)) {}

lexer_t::lexer_t(const char *next_cursor_, size_t len):
   lexer_t::lexer_t(next_cursor_, next_cursor_ + len) {}

lexer_t::lexer_t(const char *next_cursor_, const char *end_):
  next_cursor(next_cursor_),
  is_ready(false),
  cursor(nullptr),
  anchor(nullptr),
  end(end_),
  state(data),
  return_state(idle),
  current_tag_self_closing(false),
  temp_hex_reference_number(0),
  go(true) {}

void lexer_t::set_state(lexer_t::state_t state_) {
  state = state_;
}

void lexer_t::stop() {
  go = false;
}

void lexer_t::resume() {
  go = true;
}

char lexer_t::peek() {
  if (!is_ready) {
    cursor = next_cursor;
    pos = next_pos;
    char c = *next_cursor;
    switch (*cursor) {
      // normalize new line characters
      // by replacing loan \r characters with
      // \n as well as \r\n characters
      case '\r': {
        if (*(cursor + 1) == '\n') {
          ++next_cursor;
          cursor = next_cursor;
        }
        c = '\n';
        [[fallthrough]];
      }
      case '\n': {
        ++next_cursor;
        next_pos.next_line();
        break;
      }
      case '\0': {
        if (end == next_cursor) {
          break;
        }
        [[fallthrough]];
      }
      default: {
        ++next_cursor;
        next_pos.next_col();

        try {
          // std::cout << "DECODER: " << decoder << std::endl;
          auto uc = decoder(static_cast<uint8_t>(c));
          if (uc != nullptr) {
            auto n = static_cast<int>(uc[0]);
            // std::cout << "N = " << n << std::endl;
            if (is_surrogate(n)) {
              emit_parse_error("surrogate-in-input-stream");
            } else if (is_non_character(n)) {
              emit_parse_error("noncharacter-in-input-stream");
            } else if (!is_ascii_ws(n) && is_control_character(n)) {
              emit_parse_error("control-character-in-input-stream");
            }
          }
        } catch (const std::exception &e) {
          // std::cout << "DECODER EXCEPTION: " << decoder << " " << e.what() << std::endl;
          auto cp = static_cast<int>(static_cast<unsigned char>(c));
          if (is_surrogate(cp)) {
            emit_parse_error("surrogate-in-input-stream");
          } else if (is_non_character(cp)) {
            emit_parse_error("noncharacter-in-input-stream");
          } else if (cp != '\0' && !is_ascii_ws(cp) && is_control_character(cp)) {
            emit_parse_error("control-character-in-input-stream");
          }
        }
      }
    }  // switch
    is_ready = true;
    return c;
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

void lexer_t::reset_temporary_buffer() {
  temporary_buffer.str("");
  temporary_buffer.clear();
}

bool lexer_t::is_consuming_part_of_attribute() {
  if (
    return_state == attribute_value_double_quoted ||
    return_state == attribute_value_single_quoted ||
    return_state == attribute_value_unquoted
  ) {
    return true;
  }
  return false;
}

void lexer_t::flush_consumed_as_character_reference() {
  if (is_consuming_part_of_attribute()) {
    temp_tag_token->append_attribute_value(temporary_buffer.str());
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
  tag_t mutable_tag(token);
  if (mutable_tag.get_kind() == token_t::START_TAG) {
    open_tag_names.push_back(mutable_tag.get_tag_name());
  } else if (open_tag_names.size() > 0) {
    open_tag_names.pop_back();
  }
  // emit duplicate-attribute error for each duplicate tag removed
  for (int i = 0; i < mutable_tag.remove_duplicate_attributes(); ++i) {
    emit_parse_error("duplicate-attribute");
  }
  // remove all tag attributes if emitting end tag
  if (mutable_tag.get_kind() == token_t::END_TAG) {
    mutable_tag.remove_all_attributes();
    // end tags do should not have self closing flag set to on
    if (mutable_tag.is_self_closing()) {
      emit_parse_error("end-tag-with-trailing-solidus");
      mutable_tag.set_self_closing(false);
    }
  }
  this->on_tag(mutable_tag);
}

bool lexer_t::is_appropriate_end_tag() {
  return (
    open_tag_names.size() > 0 &&
    temp_tag_token->get_tag_name() == open_tag_names.back() &&
    temp_tag_token->get_kind() == token_t::END_TAG
  );
}

bool lexer_t::is_eof() {
  return end == cursor;
}

void lexer_t::lex() {
  do {
    char c = peek();
    // std::cout << "DEBUG: " << get_state_name(state) << " '" << (!isspace(c) ? c : ' ') << "' '" << codepoint(std::string{c}) << "'" << std::endl;

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
            if (is_eof()) {
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_token(character_t(pos, c));
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
            if (is_eof()) {
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
            if (is_eof()) {
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
            if (is_eof()) {
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
          if (is_eof()) {
            emit_token(eof_t(pos));
            go = false;
          } else {
            emit_token(character_t(pos, utf8chr(0xFFFD)));
            emit_parse_error("unexpected-null-character");
            pop();
          }
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
            if (is_eof()) {
              emit_parse_error("eof-before-tag-name");
              emit_token(character_t(pos, '<'));
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            if (isalpha(c)) {
              state = tag_name;
              temp_tag_token = std::make_shared<tag_t>(anchor_pos);
            } else {
              state = data;
              emit_token(character_t(pos, '<'));
              emit_parse_error("invalid-first-character-of-tag-name");
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
            if (is_eof()) {
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              emit_token(eof_t(pos));
              emit_parse_error("eof-before-tag-name");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            if (isalpha(c)) {
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            state = before_attribute_name;
            pop();
            break;
          }
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
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_tag_token->append_tag_name(utf8chr(0xFFFD));
              pop();
            }
            break;
          }
          default: {
            pop();
            if (isupper(c)) {
              temp_tag_token->append_tag_name(char(tolower(c)));
            } else {
              temp_tag_token->append_tag_name(c);
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
          state = rcdata;
          emit_token(character_t(pos, '<'));
        }
        break;
      }
      case rcdata_end_tag_open: {
        if (isalpha(c)) {
          temp_tag_token = std::make_shared<tag_t>(pos, true);
          state = rcdata_end_tag_name;
        } else {
          state = rcdata;
          emit_token(character_t(pos, c));
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
              state = self_closing_start_tag;
              pop();
            } else {
              state = rcdata;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
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
              emit_token(*temp_tag_token);
            } else {
              state = rcdata;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token, then switch
              // to the before_attribute_name state. otherwise treat is as per the anything
              // else entry below
              if (is_appropriate_end_tag()) {
                state = before_attribute_name;
                pop();
              } else {
                state = rcdata;
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
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
              state = rcdata;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
        }
        break;
      }
      case rawtext_less_than_sign: {
        if (c == '/') {
          state = rawtext_end_tag_open;
          pop();
          reset_temporary_buffer();
        } else {
          state = rawtext;
          emit_token(character_t(pos, '<'));
        }
        break;
      }
      case rawtext_end_tag_open: {
        if (isalpha(c)) {
          state = rawtext_end_tag_name;
          temp_tag_token = std::make_shared<tag_t>(pos, true);
        } else {
          state = rawtext;
          emit_token(character_t(pos, '<'));
          emit_token(character_t(pos, '/'));
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
              state = self_closing_start_tag;
              pop();
            } else {
              state = rawtext;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token,
            // then switch to the data state. Otherwise, treat
            // it as per the "anything else" entry below.
            if (is_appropriate_end_tag()) {
              state = data;
              pop();
              emit_token(*temp_tag_token);
            } else {
              state = rawtext;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token,
              // then switch to the before_attribute_name state. Otherwise, treat
              // it as per the "anything else" entry below.
              if (is_appropriate_end_tag()) {
                state = before_attribute_name;
                pop();
              } else {
                state = rawtext;
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
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
              state = rawtext;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
        }
        break;
      }
      case script_data_less_than_sign: {
        switch (c) {
          case '/': {
            state = script_data_end_tag_open;
            pop();
            reset_temporary_buffer();
            break;
          }
          case '!': {
            state = script_data_escape_start;
            pop();
            emit_token(character_t(pos, '<'));
            emit_token(character_t(pos, '!'));
            break;
          }
          default: {
            state = script_data;
            emit_token(character_t(pos, '<'));
          }
        }
        break;
      }
      case script_data_end_tag_open: {
        if (isalpha(c)) {
          state = script_data_end_tag_name;
          temp_tag_token = std::make_shared<tag_t>(pos, true);
        } else {
          emit_token(character_t(pos, '<'));
          emit_token(character_t(pos, '/'));
          state = script_data;
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
              state = self_closing_start_tag;
              pop();
            } else {
              state = script_data;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token,
            // then switch to the data state and emit the current tag token. Otherwise,
            // treat it as per the "anything else" entry below.
            if (is_appropriate_end_tag()) {
              state = data;
              pop();
              emit_token(*temp_tag_token);
            } else {
              state = script_data;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token,
              // then switch to the before attribute name state. Otherwise, treat
              // it as per the "anything else" entry below.
              if (is_appropriate_end_tag()) {
                state = before_attribute_name;
                pop();
              } else {
                state = script_data;
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
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
              state = script_data;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
          }
        }
        break;
      }
      case script_data_escape_start: {
        if (c == '-') {
          state = script_data_escape_start_dash;
          pop();
          emit_token(character_t(pos, '-'));
        } else {
          state = script_data;
        }
        break;
      }
      case script_data_escape_start_dash: {
        if (c == '-') {
          state = script_data_escaped_dash_dash;
          pop();
          emit_token(character_t(pos, '-'));
        } else {
          state = script_data;
        }
        break;
      }
      case script_data_escaped: {
        switch (c) {
          case '-': {
            state = script_data_escaped_dash;
            pop();
            emit_token(character_t(pos, '-'));
            break;
          }
          case '<': {
            state = script_data_escaped_less_than_sign;
            pop();
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-script-html-comment-like-text");
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
            state = script_data_escaped_dash_dash;
            pop();
            emit_token(character_t(pos, '-'));
            break;
          }
          case '<': {
            state = script_data_escaped_less_than_sign;
            pop();
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-script-html-comment-like-text");
              emit_token(eof_t(pos));
              go = false;
            } else {
              state = script_data_double_escaped;
              emit_parse_error("unexpected-null-character");
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              pop();
            }
            break;
          }
          default: {
            state = script_data_escaped;
            pop();
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
            emit_token(character_t(pos, c));
            break;
          }
          case '<': {
            state = script_data_escaped_less_than_sign;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '>': {
            state = script_data;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-script-html-comment-like-text");
              emit_token(eof_t(pos));
              go = false;
            } else {
              state = script_data_double_escaped;
              emit_parse_error("unexpected-null-character");
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              pop();
            }
            break;
          }
          default: {
            state = script_data_escaped;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_escaped_less_than_sign: {
        switch (c) {
          case '/': {
            state = script_data_escaped_end_tag_open;
            pop();
            reset_temporary_buffer();
            break;
          }
          default: {
            if (isalpha(c)) {
              state = script_data_double_escape_start;
              reset_temporary_buffer();
              emit_token(character_t(pos, '<'));
            } else {
              state = script_data_escaped;
              emit_token(character_t(pos, '<'));
            }
          }
        }
        break;
      }
      case script_data_escaped_end_tag_open: {
        if (isalpha(c)) {
          state = script_data_escaped_end_tag_name;
          temp_tag_token = std::make_shared<tag_t>(pos, true);
        } else {
          state = script_data_escaped;
          emit_token(character_t(pos, '<'));
          emit_token(character_t(pos, '/'));
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
              state = self_closing_start_tag;
              pop();
            } else {
              state = script_data_escaped;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          case '>': {
            // If the current end tag token is an appropriate end tag token, then switch to
            // the data state and emit the current tag token. Otherwise, treat it as per the
            // "anything else" entry below.
            if (is_appropriate_end_tag()) {
              state = data;
              pop();
              emit_token(*temp_tag_token);
            } else {
              state = script_data_escaped;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
            }
            break;
          }
          default: {
            if (isspace(c)) {
              // If the current end tag token is an appropriate end tag token, then switch
              // to the before attribute name state. Otherwise, treat it as per the "anything else"
              // entry below.
              if (is_appropriate_end_tag()) {
                state = before_attribute_name;
                pop();
              } else {
                state = script_data_escaped;
                emit_token(character_t(pos, '<'));
                emit_token(character_t(pos, '/'));
                auto str = temporary_buffer.str();
                for (auto character: str) {
                  emit_token(character_t(pos, character));
                }
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
              // Emit a U+003C LESS-THAN SIGN character token, a U+002F SOLIDUS character token, and
              // a character token for each of the characters in the temporary buffer (in the order
              // they were added to the buffer). Reconsume in the script data escaped state.
              state = script_data_escaped;
              emit_token(character_t(pos, '<'));
              emit_token(character_t(pos, '/'));
              auto str = temporary_buffer.str();
              for (auto character: str) {
                emit_token(character_t(pos, character));
              }
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
              state = script_data_double_escaped;
              pop();
            } else {
              state = script_data_escaped;
              pop();
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
                state = script_data_double_escaped;
                pop();
              } else {
                state = script_data_escaped;
                pop();
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
            state = script_data_double_escaped_dash;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '<': {
            state = script_data_double_escaped_less_than_sign;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-script-html-comment-like-text");
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              pop();
            }
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
            state = script_data_double_escaped_dash_dash;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '<': {
            state = script_data_double_escaped_less_than_sign;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-script-html-comment-like-text");
              emit_token(eof_t(pos));
              go = false;
            } else {
              state = script_data_double_escaped;
              emit_parse_error("unexpected-null-character");
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              pop();
            }
            break;
          }
          default: {
            state = script_data_double_escaped;
            pop();
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
            state = script_data_double_escaped_less_than_sign;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '>': {
            state = script_data;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-script-html-comment-like-text");
              emit_token(eof_t(pos));
              go = false;
            } else {
              state = script_data_double_escaped;
              emit_parse_error("unexpected-null-character");
              emit_token(character_t(pos, utf8chr(0xFFFD)));
              pop();
            }
            break;
          }
          default: {
            state = script_data_double_escaped;
            pop();
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case script_data_double_escaped_less_than_sign: {
        if (c == '/') {
          state = script_data_double_escape_end;
          pop();
          reset_temporary_buffer();
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
            if (str == "script") {
              state = script_data_escaped;
            } else {
              state = script_data_double_escaped;
            }
            pop();
            emit_token(character_t(pos, c));
            break;
          }
          default: {
            if (isspace(c)) {
              auto str = temporary_buffer.str();
              if (str == "script") {
                state = script_data_escaped;
              } else {
                state = script_data_double_escaped;
              }
              pop();
              emit_token(character_t(pos, c));
            } else if (isalpha(c)) {
              pop();
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // (space)
            pop();
            break;
          }
          case '/':
          case '>': {
            state = after_attribute_name;
            break;
          }
          case '=': {
            state = attribute_name;
            emit_parse_error("unexpected-equals-sign-before-attribute-name");
            temp_tag_token->start_new_attribute();
            temp_tag_token->append_attribute_name(c);
            if (temp_tag_token->get_kind() == token_t::END_TAG) {
              emit_parse_error("end-tag-with-attributes");
            }
            pop();
            break;
          }
          case '\0': {
            if (is_eof()) {
              state = after_attribute_name;
              break;
            }
            [[fallthrough]];
          }
          default: {
            temp_tag_token->start_new_attribute();
            if (temp_tag_token->get_kind() == token_t::END_TAG) {
              emit_parse_error("end-tag-with-attributes");
            }
            state = attribute_name;
            break;
          }
        }
        break;
      }
      case attribute_name: {
        switch (c) {
          case 0x0009: // tab
          case 0x000A: // lf
          case 0x000C: // ff
          case 0x000D: // cr
          case 0x0020: // space
          case '/':
          case '>': {
            state = after_attribute_name;
            break;
          }
          case '=': {
            state = before_attribute_value;
            pop();
            break;
          }
          case '"':
          case '\'':
          case '<': {
            temp_tag_token->append_attribute_name(c);
            pop();
            emit_parse_error("unexpected-character-in-attribute-name");
            break;
          }
          case '\0': {
            if (is_eof()) {
              state = after_attribute_name;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_tag_token->append_attribute_name(utf8chr(0xFFFD));
              pop();
            }
            break;
          }
          default: {
            if (isalpha(c)) {
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            pop();
            break;
          }
          case '/': {
            state = self_closing_start_tag;
            pop();
            break;
          }
          case '=': {
            state = before_attribute_value;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(*temp_tag_token);
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = attribute_name;
            temp_tag_token->start_new_attribute();
            if (temp_tag_token->get_kind() == token_t::END_TAG) {
              emit_parse_error("end-tag-with-attributes");
            }
            break;
          }
        }
        break;
      }
      case before_attribute_value: {
        switch (c) {
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            pop();
            break;
          }
          case '"': {
            state = attribute_value_double_quoted;
            pop();
            break;
          }
          case '\'': {
            state = attribute_value_single_quoted;
            pop();
            break;
          }
          case '>': {
            emit_parse_error("missing-attribute-value");
            pop();
            emit_token(*temp_tag_token);
            break;
          }
          default: {
            state = attribute_value_unquoted;
            break;
          }
        }
        break;
      }
      case attribute_value_double_quoted: {
        switch (c) {
          case '"': {
            state = after_attribute_value_quoted;
            pop();
            break;
          }
          case '&': {
            state = character_reference;
            push_state(attribute_value_double_quoted);
            pop();
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_tag_token->append_attribute_value(utf8chr(0xFFFD));
              pop();
            }
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
            state = after_attribute_value_quoted;
            pop();
            break;
          }
          case '&': {
            state = character_reference;
            push_state(attribute_value_single_quoted);
            pop();
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_tag_token->append_attribute_value(utf8chr(0xFFFD));
              pop();
            }
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            state = before_attribute_name;
            break;
          }
          case '&': {
            state = character_reference;
            pop();
            push_state(attribute_value_unquoted);
            break;
          }
          case '>': {
            state = data;
            pop();
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
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_tag_token->append_attribute_value(utf8chr(0xFFFD));
              pop();
            }
            break;
          }
          default: {
            pop();
            temp_tag_token->append_attribute_value(c);
            break;
          }
        }
        break;
      }
      case after_attribute_value_quoted: {
        switch (c) {
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            state = before_attribute_name;
            pop();
            break;
          }
          case '/': {
            state = self_closing_start_tag;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(*temp_tag_token);
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = before_attribute_name;
            emit_parse_error("missing-whitespace-between-attributes");
            break;
          }
        }
        break;
      }
      case self_closing_start_tag: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            temp_tag_token->set_self_closing(true);
            emit_token(*temp_tag_token);
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-tag");
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = before_attribute_name;
            emit_parse_error("unexpected-solidus-in-tag");
            break;
          }
        }
        break;
      }
      case bogus_comment: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            emit_token(comment_t(pos));
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_token(comment_t(pos));
              emit_token(eof_t(pos));
              go = false;
            } else {
              pop();
              emit_parse_error("unexpected-null-character");
            }
            break;
          }
          default: {
            pop();
            c = peek();
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
              state = comment_start;
              pop();
              break;
            }
            state = bogus_comment;
            emit_parse_error("incorrectly-opened-comment");
            reset_cursor(cursor_peek);
            c = peek();
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
                        state = doctype;
                        pop();
                        break;
                      }
                    }
                  }
                }
              }
            }
            state = bogus_comment;
            reset_cursor(cursor_peek);
            c = peek();
            emit_parse_error("incorrectly-opened-comment");
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
                          state = bogus_comment;
                          emit_parse_error("cdata-in-html-content");
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
            state = bogus_comment;
            reset_cursor(cursor_peek);
            c = peek();
            break;
          }
          default: {
            state = bogus_comment;
            reset_temporary_buffer();
            emit_parse_error("incorrectly-opened-comment");
            break;
          }
        }
        break;
      }
      case comment_start: {
        switch (c) {
          case '-': {
            state = comment_start_dash;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(comment_t(pos));
            emit_parse_error("abrupt-closing-of-empty-comment");
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
            state = comment_end;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(comment_t(pos));
            emit_parse_error("abrupt-closing-of-empty-comment");
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_token(comment_t(pos));
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-comment");
              go = false;
              break;
            }
            [[fallthrough]];
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
            if (is_eof()) {
              emit_parse_error("eof-in-comment");
              emit_token(comment_t(pos));
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
            if (is_eof()) {
              state = comment_end;
              break;
            }
            [[fallthrough]];
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
            if (is_eof()) {
              emit_parse_error("eof-in-comment");
              emit_token(comment_t(pos));
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
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
            if (is_eof()) {
              emit_parse_error("eof-in-comment");
              emit_token(comment_t(pos));
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
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
            state = data;
            pop();
            emit_token(comment_t(pos));
            emit_parse_error("incorrectly-closed-comment");
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-comment");
              emit_token(comment_t(pos));
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            pop();
            state = before_doctype_name;
            break;
          }
          case '>': {
            state = before_doctype_name;
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token = std::make_shared<doctype_t>(pos);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = before_doctype_name;
            emit_parse_error("missing-whitespace-before-doctype-name");
            break;
          }
        }
        break;
      }
      case before_doctype_name: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            temp_doctype_token = std::make_shared<doctype_t>(pos);
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("missing-doctype-name");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token = std::make_shared<doctype_t>(pos);
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
            } else {
              temp_doctype_token = std::make_shared<doctype_t>(pos);
              temp_doctype_token->append_doctype_name(utf8chr(0xFFFD));
              state = doctype_name;
              emit_parse_error("unexpected-null-character");
              pop();
            }
            break;
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              state = doctype_name;
              pop();
              temp_doctype_token = std::make_shared<doctype_t>(pos);
              if (isupper(c)) {
                temp_doctype_token->append_doctype_name(char(tolower(c)));
              } else {
                temp_doctype_token->append_doctype_name(c);
              }
            }
            break;
          }
        }
        break;
      }
      case doctype_name: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_doctype_token->set_force_quirks(true);
              temp_doctype_token->append_doctype_name(utf8chr(0xFFFD));
              pop();
            }
            break;
          }
          default: {
            if (isspace(c)) {
              state = after_doctype_name;
              pop();
            } else {
              pop();
              if (isupper(c)) {
                temp_doctype_token->append_doctype_name(char(tolower(c)));
              } else {
                temp_doctype_token->append_doctype_name(c);
              }
            }
            break;
          }
        }
        break;
      }
      case after_doctype_name: {
        switch (c) {
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // (space)
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
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
              reset_cursor(current_cursor);
              state = bogus_doctype;
              emit_parse_error("invalid-character-sequence-after-doctype-name");
            }
            break;
          }
        }
        break;
      }
      case after_doctype_public_keyword: {
        switch (c) {
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            pop();
            state = before_doctype_public_identifier;
            break;
          }
          case '"': {
            state = doctype_public_identifier_double_quoted;
            pop();
            temp_doctype_token->set_public_identifier("");
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            break;
          }
          case '\'': {
            pop();
            state = doctype_public_identifier_single_quoted;
            temp_doctype_token->set_public_identifier("");
            emit_parse_error("missing-whitespace-after-doctype-public-keyword");
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("missing-doctype-public-identifier");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = bogus_doctype;
            temp_doctype_token->set_force_quirks(true);
            emit_parse_error("missing-quote-before-doctype-public-identifier");
            break;
          }
        }
        break;
      }
      case before_doctype_public_identifier: {
        switch (c) {
          case '"': {
            state = doctype_public_identifier_double_quoted;
            pop();
            temp_doctype_token->set_public_identifier("");
            break;
          }
          case '\'': {
            state = doctype_public_identifier_single_quoted;
            pop();
            temp_doctype_token->set_public_identifier("");
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("missing-doctype-public-identifier");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              state = bogus_doctype;
              temp_doctype_token->set_force_quirks(true);
              emit_parse_error("missing-quote-before-doctype-public-identifier");
            }
            break;
          }
        }
        break;
      }
      case doctype_public_identifier_double_quoted: {
        switch (c) {
          case '"': {
            state = after_doctype_public_identifier;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("abrupt-doctype-public-identifier");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_doctype_token->append_public_identifier(utf8chr(0xFFFD));
              pop();
            }
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
            state = after_doctype_public_identifier;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("abrupt-doctype-public-identifier");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_doctype_token->append_public_identifier(utf8chr(0xFFFD));
              pop();
            }
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            state = between_doctype_public_and_system_identifiers;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '"': {
            state = doctype_system_identifier_double_quoted;
            pop();
            temp_doctype_token->set_system_identifier("");
            emit_parse_error("missing-whitespace-between-doctype-public-and-system-identifiers");
            break;
          }
          case '\'': {
            state = doctype_system_identifier_single_quoted;
            pop();
            temp_doctype_token->set_system_identifier("");
            emit_parse_error("missing-whitespace-between-doctype-public-and-system-identifiers");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            temp_doctype_token->set_force_quirks(true);
            state = bogus_doctype;
            emit_parse_error("missing-quote-before-doctype-system-identifier");
            break;
          }
        }
        break;
      }
      case between_doctype_public_and_system_identifiers: {
        switch (c) {
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '"': {
            state = doctype_system_identifier_double_quoted;
            temp_doctype_token->set_system_identifier("");
            pop();
            break;
          }
          case '\'': {
            state = doctype_system_identifier_single_quoted;
            pop();
            temp_doctype_token->set_system_identifier("");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              state = bogus_doctype;
              temp_doctype_token->set_force_quirks(true);
              emit_parse_error("missing-quote-before-doctype-system-identifier");
            }
            break;
          }
        }
        break;
      }
      case after_doctype_system_keyword: {
        switch (c) {
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            state = before_doctype_system_identifier;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("missing-doctype-system-identifier");
            break;
          }
          case '"': {
            state = doctype_system_identifier_double_quoted;
            temp_doctype_token->set_system_identifier("");
            pop();
            emit_parse_error("missing-whitespace-after-doctype-system-keyword");
            break;
          }
          case '\'': {
            state = doctype_system_identifier_single_quoted;
            pop();
            temp_doctype_token->set_system_identifier("");
            emit_parse_error("missing-whitespace-after-doctype-system-keyword");
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-doctype");
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = bogus_doctype;
            temp_doctype_token->set_force_quirks(true);
            emit_parse_error("missing-quote-before-doctype-system-identifier");
            break;
          }
        }
        break;
      }
      case before_doctype_system_identifier: {
        switch (c) {
          case '>': {
            state = data;
            temp_doctype_token->set_force_quirks(true);
            pop();
            emit_token(*temp_doctype_token);
            emit_parse_error("missing-doctype-system-identifier");
            break;
          }
          case '"': {
            state = doctype_system_identifier_double_quoted;
            pop();
            temp_doctype_token->set_system_identifier("");
            break;
          }
          case '\'': {
            state = doctype_system_identifier_single_quoted;
            pop();
            temp_doctype_token->set_system_identifier("");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            if (isspace(c)) {
              pop();
            } else {
              state = bogus_doctype;
              temp_doctype_token->set_force_quirks(true);
              emit_parse_error("missing-quote-before-doctype-system-identifier");
            }
            break;
          }
        }
        break;
      }
      case doctype_system_identifier_double_quoted: {
        switch (c) {
          case '"': {
            state = after_doctype_system_identifier;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("abrupt-doctype-system-identifier");
            break;
          }
          case '\0': {
            if (is_eof()) {
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-doctype");
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_doctype_token->append_system_identifier(utf8chr(0xFFFD));
              pop();
            }
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
            state = after_doctype_system_identifier;
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            temp_doctype_token->set_force_quirks(true);
            emit_token(*temp_doctype_token);
            emit_parse_error("abrupt-doctype-system-identifier");
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-doctype");
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              temp_doctype_token->append_system_identifier(utf8chr(0xFFFD));
              pop();
            }
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
          case 0x0009:   // (tab)
          case 0x000A:   // (lf)
          case 0x000C:   // (ff)
          case 0x000D:   // (cr)
          case 0x0020: { // space
            pop();
            break;
          }
          case '>': {
            state = data;
            pop();
            emit_token(*temp_doctype_token);
            break;
          }
          case '\0': {
            if (is_eof()) {
              emit_parse_error("eof-in-doctype");
              temp_doctype_token->set_force_quirks(true);
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            state = bogus_doctype;
            emit_parse_error("unexpected-character-after-doctype-system-identifier");
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
            if (is_eof()) {
              emit_token(*temp_doctype_token);
              emit_token(eof_t(pos));
              go = false;
            } else {
              emit_parse_error("unexpected-null-character");
              pop();
            }
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
            if (is_eof()) {
              emit_token(eof_t(pos));
              emit_parse_error("eof-in-cdata");
              go = false;
              break;
            }
            [[fallthrough]];
          }
          default: {
            pop();
            emit_token(character_t(pos, c));
            break;
          }
        }
        break;
      }
      case cdata_section_bracket: {
        if (c == ']') {
          state = cdata_section_end;
          pop();
        } else {
          state = cdata_section;
          emit_token(character_t(pos, c));
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
            state = data;
            pop();
            break;
          }
          default: {
            state = cdata_section;
            emit_token(character_t(pos, c));
            emit_token(character_t(pos, c));
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
          state = numeric_character_reference;
          temporary_buffer << c;
          pop();
        } else {
          // Flush code points consumed as a character reference. Reconsume in the
          // return state.
          flush_consumed_as_character_reference();
          state = pop_state();
        }
        break;
      }
      case named_character_reference: {
        std::ostringstream utf8match;
        auto match = match_entity(
          const_cast<char*>(cursor - 1),
          const_cast<char*>(end),
          utf8match
        );

        auto ustr = utf8match.str();
        if (match > -1) {
          std::string entity_text(entity_names[match]);
          for (auto iter = entity_text.begin() + 1; iter != entity_text.end(); ++iter) {
            temporary_buffer << *iter;
            pop();
            c = peek();
          }
          // If the character reference was consumed as part of an attribute, and the last
          // character matched is not a U+003B SEMICOLON character (;), and the next input
          // character is either a U+003D EQUALS SIGN character (=) or an ASCII alphanumeric,
          // then, for historical reasons, flush code points consumed as a character reference
          // and switch to the return state.
          auto last = entity_text.back();
          if (is_consuming_part_of_attribute() && last != ';' && (c == '=' || isalnum(c))) {
            flush_consumed_as_character_reference();
            state = pop_state();
          } else {
            if (last != ';') {
              emit_parse_error("missing-semicolon-after-character-reference");
            }
            reset_temporary_buffer();
            temporary_buffer << ustr;
            flush_consumed_as_character_reference();
            state = pop_state();
          }
        } else {
          flush_consumed_as_character_reference();
          state = ambiguous_ampersand;
        }
        break;
      }
      case ambiguous_ampersand: {
        if (is_ascii_alphanumeric(c)) {
          // If the character reference was consumed as part of an attribute, then
          // append the current input character to the current attribute's value. Otherwise,
          // emit the current input character as a character token.
          if (is_consuming_part_of_attribute()) {
            temp_tag_token->append_attribute_value(c);;
          } else {
            emit_token(character_t(pos, c));
          }
          pop();
        } else if (c == ';') {
          // This is an unknown-named-character-reference parse error. Reconsume in the
          // return state.
          state = pop_state();
          emit_parse_error("unknown-named-character-reference-parse-error");
        } else {
          // Reconsume in the return state
          state = pop_state();
        }
        break;
      }
      case numeric_character_reference: {
        temp_hex_reference_number = 0;
        switch (c) {
          case 'x':
          case 'X': {
            state = hexadecimal_character_reference_start;
            temporary_buffer << c;
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
          state = pop_state();
          flush_consumed_as_character_reference();
          emit_parse_error("absence-of-digits-in-numeric-character-reference");
        }
        break;
      }
      case decimal_character_reference_start: {
        if (isdigit(c)) {
          state = decimal_character_reference;
        } else {
          state = pop_state();
          flush_consumed_as_character_reference();
          emit_parse_error("absence-of-digits-in-numeric-character-reference");
        }
        break;
      }
      case hexadecimal_character_reference: {
        auto temp = temp_hex_reference_number;
        if (c == ';') {
          state = numeric_character_reference_end;
          pop();
        } else if (temp > 0x10FFFF) {
          // ignore the character
          pop();
        } else if (isdigit(c)) {
          // Multiply the character reference code by 16. Add a numeric version of
          // the current input character (subtract 0x0030 from the character's code
          // point) to the character reference code.
          pop();
          temp *= 16;
          temp += (c - 0x0030);
        } else if (isxdigit(c) && isupper(c)) {
          // Multiply the character reference code by 16. Add a numeric version
          // of the current input character as a hexadecimal digit (subtract
          // 0x0037 from the character's code point) to the character reference code.
          pop();
          temp *= 16;
          temp += (c - 0x0037);
        } else if (isxdigit(c)) {
          // Multiply the character reference code by 16. Add a numeric version of the
          // current input character as a hexadecimal digit (subtract 0x0057 from the
          // character's code point) to the character reference code.
          pop();
          temp *= 16;
          temp += (c - 0x0057);
        } else {
          state = numeric_character_reference_end;
          emit_parse_error("missing-semicolon-after-character-reference");
        }

        if (temp < temp_hex_reference_number) {
          temp = (0x10FFFF) + 1;
        }

        temp_hex_reference_number = temp;
        std::cout << "THE TEMP HEX REF NUM: " << temp_hex_reference_number << std::endl;
        break;
      }
      case decimal_character_reference: {
        switch (c) {
          case ';': {
            state = numeric_character_reference_end;
            pop();
            break;
          }
          default: {
            auto temp = temp_hex_reference_number;
            if (isdigit(c)) {
              if (temp > 0x10FFFF) {
                // ignore the character
                pop();
              } else {
                // Multiply the character reference code by 10. Add a numeric version of
                // the current input character (subtract 0x0030 from the character's code
                // point) to the character reference code.
                pop();
                temp *= 10;
                temp += (codepoint(std::string{c, 1}) - 0x0030);
              }
            } else {
              state = numeric_character_reference_end;
              emit_parse_error("missing-semicolon-after-character-reference");
            }
            if (temp < temp_hex_reference_number) {
              temp = (0x10FFFF) + 1;
            }
            temp_hex_reference_number = temp;
          }
        }
        break;
      }
      case numeric_character_reference_end: {
        switch (temp_hex_reference_number) {
          case 0x00: {
            emit_parse_error("null-character-reference");
            temp_hex_reference_number = 0xFFFD;
            break;
          }
          default: {
            std::cout << "IS IT? " << temp_hex_reference_number << std::endl;
            std::cout << "WHAT " << (temp_hex_reference_number > 0x10FFFF) << std::endl;
            if (temp_hex_reference_number > 0x10FFFF) {
              temp_hex_reference_number = 0xFFFD;
              emit_parse_error("character-reference-outside-unicode-range");
            } else if (is_surrogate(temp_hex_reference_number)) {
              temp_hex_reference_number = 0xFFFD;
              emit_parse_error("surrogate-character-reference");
            } else if (is_non_character(temp_hex_reference_number)) {
              emit_parse_error("noncharacter-character-reference");
            } else if (
              temp_hex_reference_number == 0x0D ||
              (
                is_control_character(temp_hex_reference_number) &&
                !isspace(temp_hex_reference_number)
              )
            ) {
              emit_parse_error("control-character-reference");
              switch (temp_hex_reference_number) {
                case 0x80: {
                  temp_hex_reference_number = 0x20AC;
                  break;
                }
                case 0x82: {
                  temp_hex_reference_number = 0x201A;
                  break;
                }
                case 0x83: {
                  temp_hex_reference_number = 0x0192;
                  break;
                }
                case 0x84: {
                  temp_hex_reference_number = 0x201E;
                  break;
                }
                case 0x85: {
                  temp_hex_reference_number = 0x2026;
                  break;
                }
                case 0x86: {
                  temp_hex_reference_number = 0x2020;
                  break;
                }
                case 0x87: {
                  temp_hex_reference_number = 0x2021;
                  break;
                }
                case 0x88: {
                  temp_hex_reference_number = 0x02C6;
                  break;
                }
                case 0x89: {
                  temp_hex_reference_number = 0x2030;
                  break;
                }
                case 0x8A: {
                  temp_hex_reference_number = 0x0160;
                  break;
                }
                case 0x8B: {
                  temp_hex_reference_number = 0x2039;
                  break;
                }
                case 0x8C: {
                  temp_hex_reference_number = 0x0152;
                  break;
                }
                case 0x8E: {
                  temp_hex_reference_number = 0x017D;
                  break;
                }
                case 0x91: {
                  temp_hex_reference_number = 0x2018;
                  break;
                }
                case 0x92: {
                  temp_hex_reference_number = 0x2019;
                  break;
                }
                case 0x93: {
                  temp_hex_reference_number = 0x201C;
                  break;
                }
                case 0x94: {
                  temp_hex_reference_number = 0x201D;
                  break;
                }
                case 0x95: {
                  temp_hex_reference_number = 0x2022;
                  break;
                }
                case 0x96: {
                  temp_hex_reference_number = 0x2013;
                  break;
                }
                case 0x97: {
                  temp_hex_reference_number = 0x2014;
                  break;
                }
                case 0x98: {
                  temp_hex_reference_number = 0x02DC;
                  break;
                }
                case 0x99: {
                  temp_hex_reference_number = 0x2122;
                  break;
                }
                case 0x9A: {
                  temp_hex_reference_number = 0x0161;
                  break;
                }
                case 0x9B: {
                  temp_hex_reference_number = 0x203A;
                  break;
                }
                case 0x9C: {
                  temp_hex_reference_number = 0x0153;
                  break;
                }
                case 0x9E: {
                  temp_hex_reference_number = 0x017E;
                  break;
                }
                case 0x9F: {
                  temp_hex_reference_number = 0x0178;
                  break;
                }
              }
            }
          }
        }

        reset_temporary_buffer();
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        temporary_buffer << converter.to_bytes(static_cast<char32_t>(temp_hex_reference_number));
        flush_consumed_as_character_reference();
        state = pop_state();
        break;
      }
    }
  } while (go);
}

}   // yourcss
