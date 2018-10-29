#pragma once

#include <cctype>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "error.h"
#include "ice.h"
#include "token.h"
#include "pos.h"

namespace yourhtml {

/* Convert source text into a vector of tokens. */
class lexer_t final {

public:

  /* An error in lexing. */
  class lexer_error_t final: public error_t {

  public:

    /* Report the position and what we found there. */
    lexer_error_t(const lexer_t *lexer, const char *msg);

    virtual ~lexer_error_t();

  };  // lexer_t::lexer_error_t

  enum state_t {
    idle,
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
  };

  /* Heper method to print tokens returned from lex */
  static void print_tokens(const std::vector<token_t> &tokens);

  /* Heper method to print tokens returned from lex */
  static void print_tokens(const std::vector<std::shared_ptr<token_t>> &tokens);

  /* Used by our public lex function. */
  lexer_t(const char *next_cursor);

  /* Used by our public lex function. */
  std::vector<std::shared_ptr<token_t>> lex();

  /* Set the string and position to be tokenized, and keep state */
  char reset_cursor(const char *next_);

private:

  /* Sets the return state. Throws if return state is not idle */
  void push_state(state_t return_state);

  /* Pops the return state. Throws if return state returns idle */
  state_t pop_state();

  /* Return the current character from the source text but don't advance to
     the next one. */
  char peek() const;

  /* Return the current character from the source text and advance to the
     next one. */
  char pop();

  /* Sets an anchor at the current cursor position. Throws if
     anchor is alread defined. Anchor should not be set if
     a previous anchor was dropped. */
  void set_anchor() const;

  /* Return the lexeme starting from anchor, and set anchor to null */
  std::string pop_anchor();

  /* Add a token at the current position, set anchor, advance 1 character
     and reset anchor. Used for tokens using only one character that can
     not be included in multi character tokens. ex. left_paren, right_paren
     etc.*/
  void add_single_token(token_t::kind_t kind);

  /* Temporarily holds tokens while lexing */
  std::vector<std::shared_ptr<token_t>> tokens;

  /* Our next position within the source text. */
  mutable const char *next_cursor;

  /* The (line, col) of next_cursor. */
  mutable pos_t next_pos;

  /* If true, then cursor and pos, below, are valid; otherwise,
     those fields contain junk.  Peeking makes us ready, popping makes us
     unready. */
  mutable bool is_ready;

  /* Our current position within the source text, when ready. */
  mutable const char *cursor;

  /* The (line, col) cursor, when ready. */
  mutable pos_t pos;

  /* The (line, col) cursor of an anchor. Usually the start of a lexeme. */
  mutable pos_t anchor_pos;

  /* Position in source text for anchor */
  mutable const char *anchor;

  /* The state of the lexer */
  state_t state;

  /* The return state */
  state_t return_state;

};  // lexer_t

}   // yourcss
