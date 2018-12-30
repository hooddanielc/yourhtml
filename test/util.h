#pragma once

#include <string>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <yourhtml/lexer.h>

namespace yourhtml {

namespace testfs = std::filesystem;

testfs::path get_ib_src_path();

testfs::path get_ib_project_path(const std::string &relative);

std::string read_file(const std::string &file_path);

/**
 * All tokens except characters are appended to token type
 * array.
 */
class type_accumlator_t: public lexer_t {

public:

  using lexer_t::lexer_t;

  virtual ~type_accumlator_t();

  std::vector<token_t::kind_t> token_types;

  std::vector<std::shared_ptr<token_t>> tokens;

  virtual void on_comment(const comment_t &token) {
    token_types.push_back(token.get_kind());
    tokens.push_back(std::make_shared<comment_t>(token));
  }

  virtual void on_doctype(const doctype_t &token) {
    token_types.push_back(token.get_kind());
    tokens.push_back(std::make_shared<doctype_t>(token));
  }

  virtual void on_character(const character_t &token) {
    if (!token.is_space()) {
      token_types.push_back(token.get_kind());
      tokens.push_back(std::make_shared<character_t>(token));
    }
  }

  virtual void on_eof(const eof_t &token) {
    token_types.push_back(token.get_kind());
    tokens.push_back(std::make_shared<eof_t>(token));
  }

  virtual void on_tag(const tag_t &token) {
    token_types.push_back(token.get_kind());
    tokens.push_back(std::make_shared<tag_t>(token));
  }

  virtual void on_parse_error(const type_accumlator_t::lexer_error_t &e) {
    std::cout << "LEXER ERROR: " << e.what() << std::endl;
    throw std::runtime_error(e.what());
  }

  void print_types() {
    for (const auto &kind: token_types) {
      std::cout << token_t::get_desc(kind) << std::endl;
    }
  }

  void print_tokens() {
    for (const auto &token: tokens) {
      switch (token->get_kind()) {
        case token_t::START_TAG:
        case token_t::END_TAG: {
          std::cout << token->get_as<tag_t>() << std::endl;
          break;
        }
        case token_t::DOCTYPE: {
          std::cout << token->get_as<doctype_t>() << std::endl;
          break;
        }
        case token_t::END_OF_FILE: {
          std::cout << token->get_as<eof_t>() << std::endl;
          break;
        }
        case token_t::CHARACTER: {
          std::cout << token->get_as<character_t>() << std::endl;
          break;
        }
        case token_t::COMMENT: {
          std::cout << token->get_as<comment_t>() << std::endl;
          break;
        }
      }
    }
  }

  template <typename as_t>
  as_t get_token_as(size_t i) {
    return tokens[i]->get_as<as_t>();
  }
};


class lexer_with_errors_t: public type_accumlator_t {

public:

  using type_accumlator_t::type_accumlator_t;

  std::vector<std::string> error_types;

  void on_parse_error(const lexer_error_t &error);

};  // lexer_with_errors_t

}
