#include <gtest/gtest.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

TEST(doctype_test, simple_doctype) {
  const char *src = R"(
    <!doctype html>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}

TEST(doctype_test, upper_case_simple_doctype) {
  const char *src = R"(
    <!DOCTYPE html>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}

TEST(doctype_test, weird_case_doctype) {
  const char *src = R"(
    <!DoCtYpE html>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}

TEST(doctype_test, doctype_public_and_system_identifier) {
  const char *src = R"(
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_public_identifier(), std::string("-//W3C//DTD HTML 4.01//EN"));
  EXPECT_EQ(doctype.get_system_identifier(), std::string("http://www.w3.org/TR/html4/strict.dtd"));
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}

TEST(doctype_test, doctype_html_4_01_strict_multiline) {
  const char *src = R"(
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
      "http://www.w3.org/TR/html4/strict.dtd">
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_public_identifier(), std::string("-//W3C//DTD HTML 4.01//EN"));
  EXPECT_EQ(doctype.get_system_identifier(), std::string("http://www.w3.org/TR/html4/strict.dtd"));
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}

TEST(doctype_test, xhtml_1_0_doctype) {
  const char *src = R"(
    <!DOCTYPE html
     PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_public_identifier(), std::string("-//W3C//DTD XHTML 1.0 Strict//EN"));
  EXPECT_EQ(doctype.get_system_identifier(), std::string("http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"));
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}

TEST(doctype_test, xhtml_1_0_doctype_single_quotes_and_lowercase) {
  const char *src = R"(
    <!doctype html
     public '-//W3C//DTD XHTML 1.0 Strict//EN'
     'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_EQ(doctype.get_public_identifier(), std::string("-//W3C//DTD XHTML 1.0 Strict//EN"));
  EXPECT_EQ(doctype.get_system_identifier(), std::string("http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"));
  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
}
