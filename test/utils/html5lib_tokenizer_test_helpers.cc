#include <test/utils/html5lib_tokenizer_test_helpers.h>

using namespace yourhtml;

void html5lib_test_lexer_t::on_character(const character_t &token) {
  if (tokens.size() > 0 && tokens.back()->get_kind() == token_t::CHARACTER) {
    auto data = dynamic_cast<character_t*>(tokens.back().get())->get_data();
    tokens.back() = std::make_shared<character_t>(
      tokens.back()->get_pos(),
      (data + token.get_data()).c_str()
    );
  } else {
    tokens.push_back(std::make_shared<character_t>(token));
  }
}

void html5lib_test_lexer_t::on_eof(const eof_t &) {}

void html5lib_test_lexer_t::on_parse_error(const lexer_error_t &error) {
  lexer_with_errors_t::on_parse_error(error);
  error_positions.push_back(error.get_pos());
}

html5lib_tokenizer_test_t::~html5lib_tokenizer_test_t() = default;

void print_json(const rapidjson::Value &val) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  val.Accept(writer);
}

testing::internal::ParamGenerator<html5lib_test_param_t> html5lib_test_params_in(const std::string &filename) {
  std::unordered_map<std::string, token_t::kind_t> token_kind_map({
    {"Comment", token_t::COMMENT},
    {"DOCTYPE", token_t::DOCTYPE},
    {"Character", token_t::CHARACTER},
    {"StartTag", token_t::START_TAG},
    {"EndTag", token_t::END_TAG}
  });

  FILE* fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document document;
  document.ParseStream(is);
  fclose(fp);

  if (!document["tests"].IsArray()) {
    throw std::runtime_error("no tests array in json");
  }

  auto tests = document["tests"].GetArray();
  std::vector<html5lib_test_param_t> lexer_test_params;
  for (rapidjson::SizeType i = 0; i < tests.Size(); ++i) {
    auto obj = tests[i].GetObject();
    html5lib_test_param_t test_param = {
      "",
      "",
      "",
      std::nullopt,
      std::nullopt
    };

    if (obj.HasMember("description") && obj["description"].IsString()) {
      test_param.description = obj["description"].GetString();
    } else {
      throw std::runtime_error("all lexer tests must have description string");
    }
    if (obj.HasMember("input") && obj["input"].IsString()) {
      test_param.input = std::string{
        obj["input"].GetString(),
        obj["input"].GetStringLength()
      };
    } else {
      throw std::runtime_error("all lexer tests must have input string");
    }

    test_param.id = testfs::path(filename).stem().string() + "_" + std::to_string(i);

    if (obj.HasMember("errors") && obj["errors"].IsArray()) {
      std::vector<std::pair<std::string, pos_t>> errors;
      auto errors_val = obj["errors"].GetArray();
      for (rapidjson::SizeType ei = 0; ei < errors_val.Size(); ++ei) {
        int line = 0;
        int col = 0;
        std::string code = "";
        if (!errors_val[ei].IsObject()) {
          throw std::runtime_error("errors array should contain only objects");
        }
        auto error_val = errors_val[ei].GetObject();
        if (error_val.HasMember("line") && error_val["line"].IsInt()) {
          line = error_val["line"].GetInt();
        } else {
          throw std::runtime_error("errors object must contain line int");
        }
        if (error_val.HasMember("col") && error_val["col"].IsInt()) {
          col = error_val["col"].GetInt();
        } else {
          throw std::runtime_error("errors object must contain col int");
        }
        if (error_val.HasMember("code") && error_val["code"].IsString()) {
          code = error_val["code"].GetString();
        } else {
          throw std::runtime_error("errors object must contain code int");
        }
        errors.push_back(std::make_pair(code, pos_t(line, col)));
      }
      test_param.errors = errors;
    }
    if (obj.HasMember("output") && obj["output"].IsArray()) {
      std::vector<std::shared_ptr<token_t>> tokens;
      auto output_val = obj["output"].GetArray();
      for (rapidjson::SizeType oi = 0; oi < output_val.Size(); ++oi) {
        if (!output_val[oi].IsArray()) {
          throw std::runtime_error("output array must be a list of arrays");
        }
        auto token_val = output_val[oi].GetArray();
        if (token_val.Size() >= 1 && token_val[0].IsString()) {
          std::string token_kind_str = token_val[0].GetString();
          auto kind = token_kind_map[token_kind_str];
          switch (kind) {
            case token_t::COMMENT: {
              auto token = std::make_shared<comment_t>();
              tokens.push_back(token);
              break;
            }
            case token_t::DOCTYPE: {
              auto token = std::make_shared<doctype_t>();
              tokens.push_back(token);
              break;
            }
            case token_t::CHARACTER: {
              std::string character_str;
              if (token_val.Size() < 2 || !token_val[1].IsString()) {
                throw std::runtime_error("character token in output must contain second string");
              }
              character_str = token_val[1].GetString();
              auto token = std::make_shared<character_t>(character_str);
              tokens.push_back(token);
              break;
            }
            case token_t::END_TAG:
            case token_t::START_TAG: {
              std::shared_ptr<tag_t> token;
              std::string tag_name;
              if (token_val.Size() < 2 || !token_val[1].IsString()) {
                throw std::runtime_error("start tag token in output must contain second string");
              }
              if (kind == token_t::END_TAG) {
                token = std::make_shared<tag_t>(true);
              } else {
                token = std::make_shared<tag_t>(false);  
              }
              tag_name = token_val[1].GetString();
              token->append_tag_name(tag_name);
              if (token_val.Size() >= 3) {
                if (token_val[2].IsObject()) {
                  auto attr_val = token_val[2].GetObject();
                  for (auto iter = attr_val.MemberBegin(); iter != attr_val.MemberEnd(); ++iter) {
                    token->start_new_attribute();
                    token->append_attribute_name(std::string{iter->name.GetString(), iter->name.GetStringLength()});
                    token->append_attribute_value(std::string{iter->value.GetString(), iter->value.GetStringLength()});
                  }
                } else {
                  throw std::runtime_error("tag token expected output 3rd item must be object");
                }
              }
              if (token_val.Size() >= 4) {
                if (token_val[3].IsBool()) {
                  auto self_closing = token_val[3].GetBool();
                  token->set_self_closing(self_closing);
                } else {
                  throw std::runtime_error("tag token expected output 4th item must be boolean");
                }
              }
              tokens.push_back(token);
              break;
            }
            case token_t::END_OF_FILE: {
              throw std::runtime_error("unexpected end of file in test");
            }
          }
        } else {
          throw std::runtime_error("first item in token array must be a string");
        }
      }
      test_param.output = tokens;
    }
    lexer_test_params.push_back(test_param);
  }

  return testing::ValuesIn(lexer_test_params.begin(), lexer_test_params.end());
}

std::string html5lib_tokenizer_test_title_generator_t::operator()(const testing::TestParamInfo<html5lib_test_param_t> &test_param_info) {
  return test_param_info.param.id;
}

::testing::AssertionResult HasLexerError(html5lib_test_lexer_t &lexer, const std::string &code) {
  auto iter = std::find(lexer.error_types.begin(), lexer.error_types.end(), code);
  if (iter == lexer.error_types.end()) {
    return ::testing::AssertionFailure() << "expecting error code " << code << " but found none";
  } else {
    return ::testing::AssertionSuccess() << "contains error code " << code;
  }
}

TEST_P(html5lib_tokenizer_test_t, tokenizes_as_expected) {
  auto param = GetParam();
  std::cout << "TESTING USING INPUT: ```" << std::endl;
  std::cout << "(" << param.input << std::endl << ")";
  std::cout << "```" << std::endl;
  std::cout << "TOTAL INPUT SIZE: " << param.input.size() << std::endl;
  html5lib_test_lexer_t lexer(param.input.c_str(), param.input.size());
  lexer.lex();

  if (param.errors) {
    for (auto actual_error: lexer.error_types) {
      std::cout << "ERROR ACTUAL: " << actual_error << std::endl;
    }
    for (auto expected_error: (*param.errors)) {
      std::cout << "EXPECTING ERROR: " << std::get<0>(expected_error) << std::endl;
    }
    for (auto error_type: (*param.errors)) {
      auto str = std::get<0>(error_type);
      EXPECT_TRUE(HasLexerError(lexer, str));
      // auto iter = std::find(lexer.error_types.begin(), lexer.error_types.end(), str);
      // if (iter == lexer.error_types.end()) {
      //   ::testing::AssertionFailure() << "expecting error " << str << " but found none";
      // }
    }
    if ((*param.errors).size() != lexer.error_types.size()) {
      std::cout << "Mismatching expected and actual tokenizer errors" << std::endl;
    }
  } else {
    EXPECT_EQ(lexer.error_types.size(), size_t(0));
  }

  if (param.output) {
    auto expected = (*param.output);
    auto actual = lexer.tokens;
    EXPECT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < actual.size(); ++i) {
      std::cout << "ACTUAL: " << actual[i] << std::endl;
      std::cout << "EXPECTED: " << expected[i] << std::endl;
      EXPECT_EQ(actual[i]->get_kind(), expected[i]->get_kind());
      if (actual[i]->get_kind() == expected[i]->get_kind()) {
        switch (actual[i]->get_kind()) {
          case token_t::END_TAG:
          case token_t::START_TAG: {
            auto actual_token = dynamic_cast<tag_t*>(actual[i].get());
            auto expected_token = dynamic_cast<tag_t*>(expected[i].get());
            EXPECT_EQ(*actual_token, *expected_token);
            break;
          }
          case token_t::CHARACTER: {
            auto actual_token = dynamic_cast<character_t*>(actual[i].get());
            auto expected_token = dynamic_cast<character_t*>(expected[i].get());
            EXPECT_EQ(*actual_token, *expected_token);
            break;
          }
          default: {
            //EXPECT_EQ(*actual[i], *expected[i]);
          }
        }
      }
    }
  }

  std::cout << "==========================" << std::endl;
}
