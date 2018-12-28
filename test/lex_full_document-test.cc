#include <gtest/gtest.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

TEST(lex_full_document, simple) {
  auto src = read_file("test/fixtures/simple_html_file.html");
  type_accumlator_t fixture(src.c_str());
  fixture.lex();
}

TEST(lex_full_document, github_document) {
  auto src = read_file("test/fixtures/github_html_file.html");
  type_accumlator_t fixture(src.c_str());
  fixture.lex();
}

TEST(lex_full_document, komonews_homepage) {
  auto src = read_file("test/fixtures/albino_blacksheep_homepage.html");
  type_accumlator_t fixture(src.c_str());
  fixture.lex();
}
