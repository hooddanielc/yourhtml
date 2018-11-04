#include <lick/lick.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

FIXTURE(simple) {
  auto src = read_file("test/fixtures/simple_html_file.html");
  type_accumlator_t fixture(src.c_str());
  fixture.lex();
}

FIXTURE(github_document) {
  EXPECT_OK([]() {
    auto src = read_file("test/fixtures/github_html_file.html");
    type_accumlator_t fixture(src.c_str());
    fixture.lex();
  });
}

FIXTURE(komonews_homepage) {
  auto src = read_file("test/fixtures/albino_blacksheep_homepage.html");
  type_accumlator_t fixture(src.c_str());
  fixture.lex();
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
