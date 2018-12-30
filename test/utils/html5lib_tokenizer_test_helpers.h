#pragma once
#include <optional>
#include <gtest/gtest.h>
#include <test/utils/dat.h>
#include <test/util.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

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
  std::optional<std::vector<std::shared_ptr<yourhtml::token_t>>> output;
  std::optional<std::vector<std::pair<std::string, yourhtml::pos_t>>> errors;
};

class html5lib_tokenizer_test_t : public testing::TestWithParam<html5lib_test_param_t> {

public:

  virtual ~html5lib_tokenizer_test_t();

};  // lexer_test_t

void print_json(const rapidjson::Value &val);

testing::internal::ParamGenerator<html5lib_test_param_t> html5lib_test_params_in(const std::string &filename);

struct html5lib_tokenizer_test_title_generator_t {
  std::string operator()(const testing::TestParamInfo<html5lib_test_param_t> &test_param_info);
};
