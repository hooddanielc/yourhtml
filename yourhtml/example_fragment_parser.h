#pragma once

#include <unordered_map>
#include <yourhtml/lexer.h>

namespace yourhtml {

/**
 * A parser implementation for parsing html fragments as described in the
 * HTML standard 12.4 Parsing HTML fragments
 */
class example_fragment_parser_t: public lexer_t {

public:

  enum element_kind_t {
    title,
    textarea,
    style,
    xmp,
    iframe,
    noembed,
    noframes,
    script,
    noscript,
    plaintext,
  };

  static std::unordered_map<std::string, element_kind_t> element_kind_by_name;

  example_fragment_parser_t(const char *cursor);

  virtual ~example_fragment_parser_t();

  std::vector<token_t::kind_t> token_types;

  std::vector<std::shared_ptr<token_t>> tokens;

  virtual void on_comment(const comment_t &token);

  virtual void on_doctype(const doctype_t &token);

  virtual void on_character(const character_t &token);

  virtual void on_eof(const eof_t &token);

  virtual void on_tag(const tag_t &token);

  virtual void on_parse_error(const example_fragment_parser_t::lexer_error_t &e);

  void print_types();

  void print_tokens();

  template <typename as_t>
  as_t get_token_as(size_t i) {
    return tokens[i]->get_as<as_t>();
  }

  bool get_scripting_enabled();

  void set_scripting_enabled(bool scripting);

private:

  bool scripting_enabled;

};

}
