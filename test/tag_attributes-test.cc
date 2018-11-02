#include <lick/lick.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

FIXTURE(construction) {
  const char *src = R"(
    <p style="thisisstylish">Hello :)</p>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::CHARACTER,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto start_tag = fixture.get_token_as<tag_t>(0);
  auto H = fixture.get_token_as<character_t>(1);
  auto e = fixture.get_token_as<character_t>(2);
  auto l1 = fixture.get_token_as<character_t>(3);
  auto l2 = fixture.get_token_as<character_t>(4);
  auto o = fixture.get_token_as<character_t>(5);
  auto colon = fixture.get_token_as<character_t>(6);
  auto right_paren = fixture.get_token_as<character_t>(7);
  auto end_tag = fixture.get_token_as<tag_t>(8);
  auto eof = fixture.get_token_as<eof_t>(9);

  EXPECT_EQ(H.get_data(), std::string("H"));
  EXPECT_EQ(e.get_data(), std::string("e"));
  EXPECT_EQ(l1.get_data(), std::string("l"));
  EXPECT_EQ(l2.get_data(), std::string("l"));
  EXPECT_EQ(o.get_data(), std::string("o"));
  EXPECT_EQ(colon.get_data(), std::string(":"));
  EXPECT_EQ(right_paren.get_data(), std::string(")"));
  EXPECT_EQ(start_tag.get_tag_name(), std::string("p"));
  EXPECT_EQ(end_tag.get_tag_name(), std::string("p"));

  auto attributes = start_tag.get_attributes();
  EXPECT_EQ(attributes.size(), size_t(1));
  EXPECT_EQ(std::get<0>(attributes[0]), std::string("style"));
  EXPECT_EQ(std::get<1>(attributes[0]), std::string("thisisstylish"));
}

FIXTURE(self_closing_attributes) {
  const char *src = R"(
    <p style="thisisstylish"/>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::END_OF_FILE,
  }));

  auto start_tag = fixture.get_token_as<tag_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);

  EXPECT_EQ(start_tag.get_tag_name(), std::string("p"));

  auto attributes = start_tag.get_attributes();
  EXPECT_EQ(attributes.size(), size_t(1));
  EXPECT_EQ(std::get<0>(attributes[0]), std::string("style"));
  EXPECT_EQ(std::get<1>(attributes[0]), std::string("thisisstylish"));
}

FIXTURE(multiple_attributes) {
  const char *src = R"(
    <p one="one" two="two" three="three"/>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::END_OF_FILE,
  }));

  auto start_tag = fixture.get_token_as<tag_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);

  EXPECT_EQ(start_tag.get_tag_name(), std::string("p"));
  auto attributes = start_tag.get_attributes();
  EXPECT_EQ(attributes.size(), size_t(3));
  EXPECT_EQ(std::get<0>(attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<0>(attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(attributes[2]), std::string("three"));
}

FIXTURE(weird_quotes_another_without) {
  const char *src = R"(
    <p one='one' two=two three="three"></p>
    <div one= 'one' two= two three= "three"></p>
    <img one ='one' two =two three ="three"></p>
    <href one = 'one' two = two three = "three"></p>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto first_tag = fixture.get_token_as<tag_t>(0);
  auto second_tag = fixture.get_token_as<tag_t>(2);
  auto third_tag = fixture.get_token_as<tag_t>(4);
  auto fourth_tag = fixture.get_token_as<tag_t>(6);
  auto eof = fixture.get_token_as<eof_t>(8);

  EXPECT_EQ(first_tag.get_tag_name(), std::string("p"));
  EXPECT_EQ(second_tag.get_tag_name(), std::string("div"));
  EXPECT_EQ(third_tag.get_tag_name(), std::string("img"));
  EXPECT_EQ(fourth_tag.get_tag_name(), std::string("href"));

  auto first_attributes = first_tag.get_attributes();
  auto second_attributes = second_tag.get_attributes();
  auto third_attributes = third_tag.get_attributes();
  auto fourth_attributes = fourth_tag.get_attributes();

  EXPECT_EQ(first_attributes.size(), size_t(3));
  EXPECT_EQ(std::get<0>(first_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(first_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(first_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(first_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<0>(first_attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(first_attributes[2]), std::string("three"));

  EXPECT_EQ(std::get<0>(second_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(second_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(second_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(second_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<0>(second_attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(second_attributes[2]), std::string("three"));

  EXPECT_EQ(std::get<0>(third_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(third_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(third_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(third_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<0>(third_attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(third_attributes[2]), std::string("three"));

  EXPECT_EQ(std::get<0>(fourth_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(fourth_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(fourth_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(fourth_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<0>(fourth_attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(fourth_attributes[2]), std::string("three"));
}

FIXTURE(weird_casing) {
  const char *src = R"(
    <p ONE='one' TWO=two THREE="ThReE"></p>
    <div OnE= 'one' TwO= TwO ThreE= "three"></p>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto first_tag = fixture.get_token_as<tag_t>(0);
  auto second_tag = fixture.get_token_as<tag_t>(2);
  auto eof = fixture.get_token_as<eof_t>(4);

  EXPECT_EQ(first_tag.get_tag_name(), std::string("p"));
  EXPECT_EQ(second_tag.get_tag_name(), std::string("div"));

  auto first_attributes = first_tag.get_attributes();
  auto second_attributes = second_tag.get_attributes();

  EXPECT_EQ(std::get<0>(first_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(first_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(first_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(first_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<0>(first_attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(first_attributes[2]), std::string("ThReE"));

  EXPECT_EQ(std::get<0>(second_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<1>(second_attributes[0]), std::string("one"));
  EXPECT_EQ(std::get<0>(second_attributes[1]), std::string("two"));
  EXPECT_EQ(std::get<1>(second_attributes[1]), std::string("TwO"));
  EXPECT_EQ(std::get<0>(second_attributes[2]), std::string("three"));
  EXPECT_EQ(std::get<1>(second_attributes[2]), std::string("three"));
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
