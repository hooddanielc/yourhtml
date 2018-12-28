#include <gtest/gtest.h>
#include <yourhtml/example_fragment_parser.h>
#include "util.h"

using namespace yourhtml;

TEST(fragment_parser, simple) {
  auto src = read_file("test/fixtures/simple_html_file.html");
  example_fragment_parser_t fixture(src.c_str());
  fixture.lex();
}

TEST(fragment_parser, github_document) {
  auto src = read_file("test/fixtures/github_html_file.html");
  example_fragment_parser_t fixture(src.c_str());
  fixture.lex();
}

TEST(fragment_parser, albino_blacksheep_homepage) {
  auto src = read_file("test/fixtures/albino_blacksheep_homepage.html");
  example_fragment_parser_t fixture(src.c_str());
  fixture.lex();
}
