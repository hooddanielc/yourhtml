#pragma once

#include <yourhtml/example_fragment_parser.h>

namespace yourhtml {

int count = 0;

/**
 * A parser implementation for parsing html fragments as described in the
 * HTML standard 12.4 Parsing HTML fragments
 */
example_fragment_parser_t::~example_fragment_parser_t() = default;

example_fragment_parser_t::example_fragment_parser_t(const char *cursor):
    lexer_t(cursor),
    scripting_enabled(true) {count = 0;}

void example_fragment_parser_t::on_comment(const comment_t &token) {
  token_types.push_back(token.get_kind());
  tokens.push_back(std::make_shared<comment_t>(token));
}

void example_fragment_parser_t::on_doctype(const doctype_t &token) {
  token_types.push_back(token.get_kind());
  tokens.push_back(std::make_shared<doctype_t>(token));
}

void example_fragment_parser_t::on_character(const character_t &token) {
  if (!token.is_space()) {
    token_types.push_back(token.get_kind());
    tokens.push_back(std::make_shared<character_t>(token));
  }
}

void example_fragment_parser_t::on_eof(const eof_t &token) {
  token_types.push_back(token.get_kind());
  tokens.push_back(std::make_shared<eof_t>(token));
}

std::unordered_map<std::string, example_fragment_parser_t::element_kind_t> example_fragment_parser_t::element_kind_by_name = {
  {"title", example_fragment_parser_t::title},
  {"textarea", example_fragment_parser_t::textarea},
  {"style", example_fragment_parser_t::style},
  {"xmp", example_fragment_parser_t::xmp},
  {"iframe", example_fragment_parser_t::iframe},
  {"noembed", example_fragment_parser_t::noembed},
  {"noframes", example_fragment_parser_t::noframes},
  {"script", example_fragment_parser_t::script},
  {"noscript", example_fragment_parser_t::noscript},
  {"plaintext", example_fragment_parser_t::plaintext},
};

void example_fragment_parser_t::on_tag(const tag_t &token) {  
  // if (token.get_kind() == token_t::START_TAG) {
  //   for (int i = 0; i < count; ++i) {
  //     std::cout << " ";
  //   }
  //   std::cout << token << std::endl;
  //   if (!token.is_self_closing()) {
  //     count += 2;
  //   }
  // } else {
  //   count -= 2;
  //   for (int i = 0; i < count; ++i) {
  //     std::cout << " ";
  //   }
  //   std::cout << token << std::endl;
  // }
  if (token.get_kind() == token_t::START_TAG) {
    const auto &find_kind = element_kind_by_name.find(token.get_tag_name());
    if (find_kind != element_kind_by_name.end()) {
      auto kind = find_kind->second;
      switch (kind) {
        case title:
        case textarea: {
          this->set_state(lexer_t::rcdata);
          break;
        }
        case style:
        case xmp:
        case iframe:
        case noembed:
        case noframes: {
          this->set_state(lexer_t::rawtext);
          break;
        }
        case script: {
          this->set_state(lexer_t::script_data);
          break;
        }
        case noscript: {
          if (scripting_enabled) {
            this->set_state(lexer_t::rawtext);
          }
          break;
        }
        case plaintext: {
          this->set_state(lexer_t::plaintext);
          break;
        }
      }
    }
  }
  token_types.push_back(token.get_kind());
  tokens.push_back(std::make_shared<tag_t>(token));
}

void example_fragment_parser_t::on_parse_error(const example_fragment_parser_t::lexer_error_t &e) {
  std::cout << "LEXER ERROR: " << e.what() << std::endl;
  throw std::runtime_error(e.what());
}

bool example_fragment_parser_t::get_scripting_enabled() {
  return scripting_enabled;
}

void example_fragment_parser_t::set_scripting_enabled(bool scripting_enabled_) {
  scripting_enabled = scripting_enabled_;
}

void example_fragment_parser_t::print_types() {
  for (const auto &kind: token_types) {
    std::cout << token_t::get_desc(kind) << std::endl;
  }
}

void example_fragment_parser_t::print_tokens() {
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

}
