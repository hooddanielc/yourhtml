#include <gtest/gtest.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

TEST(tag_test, open_and_close) {
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

TEST(tag_test, uppercase) {
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

TEST(tag_test, self_closing) {
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

TEST(tag_test, self_closing_child) {
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

inline auto get_subject_with_errors(const char *src) {
  auto subject = std::make_shared<lexer_with_errors_t>(src);
  subject->lex();
  return subject;
}

TEST(tag_test, tag_open_unexpected_question_mark_instead_of_tag_name_error) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <?head>
  )");

  EXPECT_EQ(subject->tokens[1]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "unexpected-question-mark-instead-of-tag-name");
}

TEST(tag_test, tag_open_invalid_first_character) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <1thug>
  )");

  EXPECT_EQ(subject->tokens[1]->get_kind(), token_t::kind_t::CHARACTER);
  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "invalid-first-character-of-tag-name");
}

TEST(tag_test, tag_open_eof_before_tag_name) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <)"
  );

  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "eof-before-tag-name");
}

TEST(tag_test, end_tag_open_missing_end_tag_name) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <bad> cool </> </bad>
  )");

  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "missing-end-tag-name");
}

TEST(tag_test, end_tag_open_eof_error) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <what></)"
  );

  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "eof-before-tag-name");
}

TEST(tag_test, end_tag_open_invalid_first_character_of_tag_name) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <what></1what>
  )");

  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "invalid-first-character-of-tag-name");
}

TEST(tag_test, tag_name_eof_in_tag_name) {
  auto subject = get_subject_with_errors(R"(
    <!doctype html>
    <wha)"
  );

  EXPECT_EQ(subject->error_types.size(), size_t(1));
  EXPECT_EQ(subject->error_types[0], "eof-in-tag");
}
