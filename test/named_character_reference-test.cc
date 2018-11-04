#include <locale>
#include <codecvt>
#include <lick/lick.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

FIXTURE(simple) {
  const char *src = R"(
    <!doctype html>
    <html>
      &nbsp; &leftrightarrow;&euro;
    </html>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::START_TAG,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto first_char = fixture.get_token_as<character_t>(2);
  auto second_char = fixture.get_token_as<character_t>(3);
  auto third_char = fixture.get_token_as<character_t>(4);

  std::cout << first_char.get_data() << std::endl;
  std::cout << second_char.get_data() << std::endl;
  std::cout << third_char.get_data() << std::endl;

  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  EXPECT_EQ(std::string(first_char.get_data()), converter.to_bytes(160));
  EXPECT_EQ(std::string(second_char.get_data()), converter.to_bytes(8596));
  EXPECT_EQ(std::string(third_char.get_data()), converter.to_bytes(8364));
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
