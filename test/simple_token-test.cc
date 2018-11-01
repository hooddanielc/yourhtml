#include <lick/lick.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

FIXTURE(construction) {
  const char *src = R"(
    <!doctype html>
    <html>
      :)
    </html>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::START_TAG,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto start_tag = fixture.get_token_as<tag_t>(1);
  auto first_char = fixture.get_token_as<character_t>(2);
  auto second_char = fixture.get_token_as<character_t>(3);
  auto end_tag = fixture.get_token_as<tag_t>(4);

  EXPECT_EQ(doctype.get_doctype_name(), std::string("html"));
  EXPECT_EQ(start_tag.get_tag_name(), std::string("html"));
  EXPECT_EQ(std::string(first_char.get_data()), std::string(":"));
  EXPECT_EQ(std::string(second_char.get_data()), std::string(")"));
  EXPECT_EQ(end_tag.get_tag_name(), std::string("html"));
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
