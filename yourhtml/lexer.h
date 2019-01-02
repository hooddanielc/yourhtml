#pragma once

#include <cstring>
#include <cctype>
#include <iostream>
#include <map>
#include <vector>
#include <memory>

#include "error.h"
#include "ice.h"
#include "token.h"
#include "pos.h"
#include "lookup_character_reference.h"

#include "tokens/character.h"
#include "tokens/comment.h"
#include "tokens/doctype.h"
#include "tokens/tag.h"
#include "tokens/eof.h"
#include "util.h"
#include <yourhtml_entities/entity_lexer.h>


namespace yourhtml {

/* Convert source text into a vector of tokens. */
class lexer_t {

public:

  /* An error in lexing. */
  class lexer_error_t final: public error_t {

  public:

    /* Report the position and what we found there. */
    lexer_error_t(const lexer_t *lexer, const char *msg);

    virtual ~lexer_error_t();

    std::string get_type() const;

  private:

    std::string type;

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
    comment_less_than_sign,
    comment_less_than_sign_bang,
    comment_less_than_sign_bang_dash,
    comment_less_than_sign_bang_dash_dash,
    comment_end_dash,
    comment_end,
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
    character_reference,
    named_character_reference,
    ambiguous_ampersand,
    numeric_character_reference,
    hexadecimal_character_reference_start,
    decimal_character_reference_start,
    hexadecimal_character_reference,
    decimal_character_reference,
    numeric_character_reference_end,
  };

  std::string get_state_name(state_t state);

  void emit_token(const comment_t &token);

  void emit_token(const doctype_t &token);

  void emit_token(const character_t &token);

  void emit_token(const eof_t &token);

  void emit_token(const tag_t &token);

  void emit_parse_error(const std::string &);

  /* Heper method to print tokens returned from lex */
  static void print_tokens(const std::vector<token_t> &tokens);

  /* Heper method to print tokens returned from lex */
  static void print_tokens(const std::vector<std::shared_ptr<token_t>> &tokens);

  /* Used by our public lex function. */
  lexer_t(const char *next_cursor);

  lexer_t(const char *next_cursor, size_t len);

  /* To conform to HTML living standard, this lexer supports input
     containing multiple null characters (U+0000). For most
     applications, this is not needed. To handle this,
     provide the string length in contructor so it can
     safely emit null characters. */
  lexer_t(const char *next_cursor, const char *end);

  virtual ~lexer_t();

  /* Tell the lexer to start consuming input from the current cursor
     until there is no more input or stopped by the user (TODO). When
     modifying existing states, be careful where the change in state
     is placed. It is important that the state is modified BEFORE
     the token is emitted. Assigning state after emitting a token
     will prevent the parser from delegating state, which is required
     for all parser algorithm implementations as described in the
     html standard. */
  void lex();

  /* Set next state the tokenizer should transition to in lex function */
  void set_state(state_t state);

  /* Set the string and position to be tokenized, and keep state. */
  char reset_cursor(const char *next_);

  /* Prevent lexer from consuming more input in lex function. */
  void stop();

  /* Resume parsing input from the current cursor in next in lex function.
     lex must be called if it has stopped. */
  void resume();

  /* Return true if their are no more characters in the input stream.
     This is required by html spec. */
  bool is_eof();

  virtual void on_comment(const comment_t &) {}

  virtual void on_doctype(const doctype_t &) {}

  virtual void on_character(const character_t &) {}

  virtual void on_eof(const eof_t &) {}

  virtual void on_tag(const tag_t &) {}

  virtual void on_parse_error(const lexer_error_t &) {}

private:

  /* Sets the return state. Throws if return state is not idle */
  void push_state(state_t return_state);

  /* Pops the return state. Throws if return state returns idle */
  state_t pop_state();

  /* Return the current character from the source text but don't advance to
     the next one. */
  char peek();

  /* Return the current character from the source text and advance to the
     next one. */
  char pop();

  /* Sets an anchor at the current cursor position. Throws if
     anchor is alread defined. Anchor should not be set if
     a previous anchor was dropped. */
  void set_anchor() const;

  /* Return the lexeme starting from anchor, and set anchor to null */
  std::string pop_anchor();

  /* Reset the temporary buffer */
  void reset_temporary_buffer();

  /* Checks if current temp_tag_token is an appropriate end tag token whose tag
     name matches the tag name of the last start tag to have been emitted from this
     tokenizer, if any. If no start tag has been emitted from this tokenizer, then
     no end tag token is appropriate. */
  bool is_appropriate_end_tag();

  /* A character reference is said to be consumed as part of an attribute
     if the return state is either attribute value (double-quoted) state,
     attribute value (single-quoted) state or attribute value (unquoted)
     state. */
  bool is_consuming_part_of_attribute();

  /* When a state says to flush code points consumed as a character reference,
     it means that for each code point in the temporary buffer (in the order
     they were added to the buffer) user agent must append the code point from
     the buffer to the current attribute's value if the character reference was
     consumed as part of an attribute, or emit the code point as a character
     token otherwise. */
  void flush_consumed_as_character_reference();

  /* Set the current tag as self closing or not self closing */
  void set_current_tag_self_closing(bool);

  /* Temporarily holds tokens while lexing */
  std::vector<token_t> tokens;

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

  /* The last cursor position for the current buffer */
  mutable const char *end;

  /* The state of the lexer */
  state_t state;

  /* The return state */
  state_t return_state;

  /* Temporary buffer */
  std::stringstream temporary_buffer;

  /* Attribute value buffer */
  std::stringstream attribute_value_buffer;

  /* Current tag token */
  std::vector<std::shared_ptr<token_t>> stack_of_open_elements;

  /* Token in the process of being parsed */
  std::shared_ptr<token_t> current_token;

  /* The current tag's attributes */
  std::vector<std::pair<std::string, std::string>> current_token_attributes;

  /* Is the current tag self closing? */
  bool current_tag_self_closing;

  std::shared_ptr<doctype_t> temp_doctype_token;

  std::shared_ptr<tag_t> temp_tag_token;

  std::shared_ptr<comment_t> temp_comment_token;

  int temp_hex_reference_number;

  bool go;

  std::vector<std::string> open_tag_names;

};  // lexer_t

}   // yourcss
