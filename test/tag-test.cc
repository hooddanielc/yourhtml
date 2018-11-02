#include <lick/lick.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

FIXTURE(open_and_close) {
  const char *src = R"(
    <p></p>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto start_tag = fixture.get_token_as<tag_t>(0);
  auto end_tag = fixture.get_token_as<tag_t>(1);

  EXPECT_EQ(start_tag.get_tag_name(), std::string("p"));
  EXPECT_FALSE(start_tag.is_self_closing());
  EXPECT_EQ(end_tag.get_tag_name(), std::string("p"));
  EXPECT_FALSE(end_tag.is_self_closing());
}

FIXTURE(uppercase) {
  const char *src = R"(
    <DIV></DIV>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto start_tag = fixture.get_token_as<tag_t>(0);
  auto end_tag = fixture.get_token_as<tag_t>(1);

  EXPECT_EQ(start_tag.get_tag_name(), std::string("div"));
  EXPECT_FALSE(start_tag.is_self_closing());
  EXPECT_EQ(end_tag.get_tag_name(), std::string("div"));
  EXPECT_FALSE(end_tag.is_self_closing());
}

FIXTURE(self_closing) {
  const char *src = R"(
    <DIV/>
    <ImG />
    <dIv attr='asdf' />
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::START_TAG,
    token_t::START_TAG,
    token_t::END_OF_FILE,
  }));

  auto one_tag = fixture.get_token_as<tag_t>(0);
  auto two_tag = fixture.get_token_as<tag_t>(1);
  auto three_tag = fixture.get_token_as<tag_t>(2);

  EXPECT_EQ(one_tag.get_tag_name(), std::string("div"));
  EXPECT_TRUE(one_tag.is_self_closing());
  EXPECT_EQ(two_tag.get_tag_name(), std::string("img"));
  EXPECT_TRUE(two_tag.is_self_closing());
  EXPECT_EQ(three_tag.get_tag_name(), std::string("div"));
  EXPECT_TRUE(three_tag.is_self_closing());
}

FIXTURE(self_closing_child) {
  const char *src = R"(
    <div>
      <img src="http://google.com" />
    </div>
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::START_TAG,
    token_t::START_TAG,
    token_t::END_TAG,
    token_t::END_OF_FILE,
  }));

  auto one_tag = fixture.get_token_as<tag_t>(0);
  auto two_tag = fixture.get_token_as<tag_t>(1);
  auto three_tag = fixture.get_token_as<tag_t>(2);

  EXPECT_EQ(one_tag.get_tag_name(), std::string("div"));
  EXPECT_FALSE(one_tag.is_self_closing());
  EXPECT_EQ(two_tag.get_tag_name(), std::string("img"));
  EXPECT_TRUE(two_tag.is_self_closing());
  EXPECT_EQ(three_tag.get_tag_name(), std::string("div"));
  EXPECT_FALSE(three_tag.is_self_closing());
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
