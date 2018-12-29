#include <gtest/gtest.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>
#include "util.h"

using namespace yourhtml;

inline std::shared_ptr<lexer_with_errors_t> get_subject(const char *src) {
  auto fixture = std::make_shared<lexer_with_errors_t>(src);
  fixture->lex();
  return fixture;
}

TEST(lex_comment_states, construction) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- This is a regular comment -->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_no_content) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!---->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_start_abdrupt_closing_of_empty_comment_error) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "abrupt-closing-of-empty-comment");
}

TEST(lex_comment_states, comment_start_dash_abdrupt_closing_of_empty_comment_error) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!--->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "abrupt-closing-of-empty-comment");
}

TEST(lex_comment_states, comment_eof_in_comment) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- eof in comment
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-comment");
}

TEST(lex_comment_states, comment_start_dash_eof_in_comment) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!---)"
  );

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-comment");
}

TEST(lex_comment_states, comment_start_dash_default_state) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!--- extra dash ok -->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_less_than_ok) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!--<<this is ok>>-->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_less_than_bang_end) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this is weird but it could happen <!-this is ok -->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_less_than_sign_bang_dash_dash_nested_comment_error) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this is weird but it could happen <!--this not that ok -->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "nested-comment");
}

TEST(lex_comment_states, comment_end_dash_eof_in_comment_error) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this not ok -)"
  );

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-comment");
}

TEST(lex_comment_states, comment_end_extra_dashes) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this is more than ok --------->
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_end_bang_comment_end_ok) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this is almost not ok --!-->
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(lex_comment_states, comment_end_eof_in_comment_error) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this not ok --)"
  );

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-comment");
}

TEST(lex_comment_states, comment_end_bang_incorrectly_closed_comment_error) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this not ok, but kind of looks ok --!>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "incorrectly-closed-comment");
}

TEST(lex_comment_states, comment_end_bang_eof_in_comment) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this not ok, but kind of looks ok --!)"
  );

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-comment");
}

TEST(lex_comment_states, comment_end_bang_ok) {
  auto fixture = get_subject(R"(
    <!doctype html>
    <html>
      <!-- this is almost not ok --! -->
    </html>
  )");

  EXPECT_EQ(fixture->tokens[2]->get_kind(), token_t::kind_t::COMMENT);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}
