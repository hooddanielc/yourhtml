#pragma once

#include <cctype>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <sstream>

namespace yourhtml {

/* Daaaaang, look at dat lexer */
class dat_lexer_t final {

public:

  class pos_t final {

  public:

    pos_t() noexcept;

    void next_col();

    void next_line();

    int get_line() const;

    int get_col() const;

    friend std::ostream &operator<<(std::ostream &strm, const pos_t &that);

  private:

    int line_number, col_number;

  };  // pos_t


  /* The base for all the kinds of errors we throw. */
  class error_t: public std::exception {

  public:

    /* Return our diagnostic message. */
    virtual const char *what() const noexcept override final;

    virtual ~error_t() override;

  protected:

    /* Do-little. */
    error_t(const pos_t &pos);

    /* Mark the end of a section of a diagnostic message.  The classes which
       inehrit from us will use this in their constructors. */
    void end_section();

    /* The stream to which to write our diagnostic message.  The classes which
       inehrit from us will use this in their constructors. */
    std::ostream &get_strm() const;

  private:

    /* A string-builder we use to build our diagnostic message. */
    mutable std::unique_ptr<std::ostringstream> strm;

    /* If true, then the next time we add to the message, we'll insert a
       separator character first. */
    mutable bool sep_needed;

    /* Our diagnostic message. */
    mutable std::string msg;

  };  // dat_lexer_t::error_t

  /* An error in lexing. */
  class lexer_error_t final: public error_t {

  public:

    /* Report the position and what we found there. */
    lexer_error_t(const dat_lexer_t *dat_lexer, const char *msg);

    virtual ~lexer_error_t();

  };  // dat_lexer_t::lexer_error_t

  /* The dreaded internal compiler error. */
  class ice_t final: public error_t {

  public:

    /* Report the file and line at which we iced. */
    ice_t(const pos_t &pos, const char *file, int line_number);

    virtual ~ice_t() override;

  };  // dat_lexer_t::ice_t

  class token_t {

  public:

    enum kind_t {
      TODO,
    };   // kind_t

    /* Cache the kind. */
    token_t(kind_t kind);

    /* Cache the position and kind and set the text to the empty string. */
    token_t(const pos_t &pos, kind_t kind);

    /* Cache the position and kind and the text. */
    token_t(const pos_t &pos, kind_t kind, std::string &&text);

    static std::string get_desc(kind_t kind);

    kind_t get_kind() const;

    std::string get_text() const;

    std::string get_name() const;

    pos_t get_pos() const;

    static std::shared_ptr<token_t> make(kind_t kind);

    static std::shared_ptr<token_t> make(const pos_t &pos, kind_t kind);

    static std::shared_ptr<token_t> make(const pos_t &pos, kind_t kind, std::string &&text);

  protected:

    /* Writes a human-readable dump of the token.  This is for debugging
     purposes only. In production, a user never sees tokens directly. */
    friend std::ostream &operator<<(std::ostream &strm, const token_t &that);

    friend std::ostream &operator<<(std::ostream &strm, const token_t *that);

    /* See accessors. */
    pos_t pos;

    /* See accessor. */
    kind_t kind;

    /* See accessor. */
    std::string text;

  };  // dat_lexer_t::token_t

  /* Heper method to print tokens returned from lex */
  static void print_tokens(const std::vector<token_t> &tokens);

  /* Used by our public lex function. */
  dat_lexer_t(const char *next_cursor);

  /* Used by our public lex function. */
  std::vector<std::shared_ptr<token_t>> lex();

private:

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

};  // dat_lexer_t

}   // yourhtml
