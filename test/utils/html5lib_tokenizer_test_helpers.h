#pragma once
#include <cstdio>
#include <optional>
#include <algorithm>
#include <functional>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>
#include <gtest/gtest.h>
#include <test/utils/dat.h>
#include <test/util.h>
#include <test/utils/ignored_tests.h>

class html5lib_test_lexer_t: public yourhtml::lexer_with_errors_t {

public:

  using yourhtml::lexer_with_errors_t::lexer_with_errors_t;

  std::vector<yourhtml::pos_t> error_positions;

  /* join consecutive character tokens */
  virtual void on_character(const yourhtml::character_t &token);

  /* ignore eof_t */
  virtual void on_eof(const yourhtml::eof_t &);

  /* log error positions */
  virtual void on_parse_error(const yourhtml::lexer_t::lexer_error_t &error);

};  // html5lib_test_lexer_t

struct html5lib_test_param_t {
  std::string description;
  std::string input;
  std::string id;
  std::optional<std::vector<std::shared_ptr<yourhtml::token_t>>> output;
  std::optional<std::vector<std::pair<std::string, yourhtml::pos_t>>> errors;
  std::optional<const char *> replacement_chars;
  std::optional<const char *> form_feed_chars;
  std::optional<bool> ignored;

  friend std::ostream& operator<<(std::ostream &os, const html5lib_test_param_t &that) {
    os << "```" << std::endl;
    os << "  html5lib_test_param_t {" << std::endl;
    os << "    description = " << that.description << ";" << std::endl;
    os << "    input = " << that.input << ";" << std::endl;
    if (that.output) {
      os << "    output = " << (*that.output) << std::endl;
    }
    if (that.errors) {
      os << "    errors = ";
      for (auto item: *that.errors) {
        os << std::get<1>(item) << " " << std::get<0>(item);
      }
      os << std::endl;
    }
    os << "  }" << std::endl;
    os << "```" << std::endl;
    return os;
  }
};

class html5lib_tokenizer_test_t : public testing::TestWithParam<html5lib_test_param_t> {

public:

  virtual ~html5lib_tokenizer_test_t();

};  // lexer_test_t

void print_json(const rapidjson::Value &val);

testing::internal::ParamGenerator<html5lib_test_param_t> html5lib_test_params_in(const std::string &filename, const std::string &test_key = "tests");

struct html5lib_tokenizer_test_title_generator_t {
  std::string operator()(const testing::TestParamInfo<html5lib_test_param_t> &test_param_info);
};

::testing::AssertionResult has_lexer_error(html5lib_test_lexer_t &lexer, const std::string &code);
