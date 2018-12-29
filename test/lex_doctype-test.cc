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

TEST(doctype_test, doctype_only_system_identifier) {
  const char *src = R"(
    <!DOCTYPE document SYSTEM "subjects.dtd">
  )";
  type_accumlator_t fixture(src);
  fixture.lex();

  EXPECT_TRUE(fixture.token_types == std::vector<token_t::kind_t>({
    token_t::DOCTYPE,
    token_t::END_OF_FILE,
  }));

  auto doctype = fixture.get_token_as<doctype_t>(0);
  auto eof = fixture.get_token_as<eof_t>(1);
  EXPECT_TRUE(doctype.is_missing_public_identifier());
  EXPECT_EQ(doctype.get_system_identifier(), std::string("subjects.dtd"));
  EXPECT_EQ(doctype.get_doctype_name(), std::string("document"));
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

inline std::shared_ptr<lexer_with_errors_t> get_subject_with_errors(const char *src) {
  auto fixture = std::make_shared<lexer_with_errors_t>(src);
  fixture->lex();
  return fixture;
}

TEST(doctype_test, before_doctype_name_missing_doctype_name_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype>
    <html>
    </html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-doctype-name");
}

TEST(doctype_test, doctype_eof_in_doctype_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, doctype_missing_whitespace_before_doctype_name) {
  auto fixture = get_subject_with_errors(R"(
    <!doctypehtml>
    <html>
    </html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-before-doctype-name");
}

TEST(doctype_test, before_doctype_name_eof_in_doctype) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype )"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, before_doctype_name_extra_space_ok) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype  html>
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(doctype_test, doctype_name_eof_in_doctype_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype h)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, after_doctype_name_with_space_ok) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html >
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(doctype_test, after_doctype_name_eof_in_doctype) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html )"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, bogus_doctype_eof_in_system_keyword) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html pub)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "invalid-character-sequence-after-doctype-name");
}





TEST(doctype_test, after_doctype_public_keyword_double_quotes_missing_ws_public_keyword_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public"totallycool" "http://asdf.com">
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-after-doctype-public-keyword");
}

TEST(doctype_test, after_doctype_public_keyword_single_quotes_missing_ws_public_keyword_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public'totallycool' "http://asdf.com">
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-after-doctype-public-keyword");
}

TEST(doctype_test, after_doctype_public_keyword_missing_doctype_public_ident) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public>
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-doctype-public-identifier");
}

TEST(doctype_test, after_doctype_public_keyword_eof_in_doctype) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, after_doctype_public_keyword_to_bogus) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html publicBOGUS>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-quote-before-doctype-public-identifier");
}

TEST(doctype_test, after_doctype_system_keyword_double_quotes_missing_ws_public_keyword_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system"totallycool">
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);

  for (auto s: fixture->error_types) {
    std::cout << s << std::endl;
  }

  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-after-doctype-system-keyword");
}

TEST(doctype_test, after_doctype_system_keyword_single_quotes_missing_ws_public_keyword_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system'totallycool'>
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-after-doctype-system-keyword");
}

TEST(doctype_test, after_doctype_system_keyword_missing_doctype_public_ident) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system>
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-doctype-system-identifier");
}

TEST(doctype_test, after_doctype_system_keyword_eof_in_doctype) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, after_doctype_system_keyword_to_bogus) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html systemBOGUS>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-quote-before-doctype-system-identifier");
}

TEST(doctype_test, before_doctype_system_identifier_missing_doctype_public_identifier) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system >
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-doctype-system-identifier");
}

TEST(doctype_test, before_doctype_system_identifier_eof_in_doctype) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system )"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, before_doctype_system_identifier_lots_of_spaces) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system   'lotsofspaces' >
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(doctype_test, before_doctype_system_identifier_missing_quote_before_system_ident) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system asdf>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-quote-before-doctype-system-identifier");
}

TEST(doctype_test, before_doctype_public_identifier_eof_in_doctype) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public )"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, before_doctype_public_identifier_missing_doctype_public_ident) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public >
    <html></html>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-doctype-public-identifier");
}

TEST(doctype_test, before_doctype_public_identifier_lots_of_spaces) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public   'lotsofspaces'    'asdf'   >
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(doctype_test, before_doctype_public_identifier_missing_quote_before_public_ident) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public asdf asdf>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-quote-before-doctype-public-identifier");
}

TEST(doctype_test, doctype_public_identifier_double_quoted_abrupt_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public "asdf>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "abrupt-doctype-public-identifier");
}

TEST(doctype_test, doctype_public_identifier_double_quoted_eof_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public "asdf)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}



TEST(doctype_test, doctype_system_identifier_double_quoted_abrupt_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system "asdf>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "abrupt-doctype-system-identifier");
}

TEST(doctype_test, doctype_system_identifier_double_quoted_eof_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system "asdf)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, doctype_system_identifier_single_quoted_abrupt_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system 'asdf>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "abrupt-doctype-system-identifier");
}

TEST(doctype_test, doctype_system_identifier_single_quoted_eof_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system 'asdf)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, doctype_public_identifier_single_quoted_abrupt_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "abrupt-doctype-public-identifier");
}

TEST(doctype_test, doctype_public_identifier_single_quoted_eof_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf)"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, after_doctype_public_identifier_end) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf'>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(doctype_test, after_doctype_public_identifier_double_missing_space_before_system) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf'"double system">
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-between-doctype-public-and-system-identifiers");
}

TEST(doctype_test, after_doctype_public_identifier_single_missing_space_before_system) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf''double system'>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-whitespace-between-doctype-public-and-system-identifiers");
}

TEST(doctype_test, after_doctype_public_identifier_eof_in_doctype_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf')"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, after_doctype_public_identifier_missing_system_quotes_is_bogus) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf'MissingSystemQuotesIsBogus>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-quote-before-doctype-system-identifier");
}


TEST(doctype_test, after_doctype_system_identifier_eof_in_doctype_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system 'asdf')"
  );

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, after_doctype_system_identifier_unexpected_character) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html system 'asdf'MissingSystemQuotesIsBogus>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "unexpected-character-after-doctype-system-identifier");
}

TEST(doctype_test, between_doctype_public_and_system_identifiers_extra_space_after_public_ident_ok) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf' >
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(0));
}

TEST(doctype_test, between_doctype_public_and_system_identifiers_eof_error) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf' 
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "eof-in-doctype");
}

TEST(doctype_test, between_doctype_public_and_system_identifiers_missing_quote_before_sys_ident) {
  auto fixture = get_subject_with_errors(R"(
    <!doctype html public 'asdf' system>
  )");

  EXPECT_EQ(fixture->tokens[0]->get_kind(), token_t::kind_t::DOCTYPE);
  EXPECT_EQ(fixture->error_types.size(), size_t(1));
  EXPECT_EQ(fixture->error_types[0], "missing-quote-before-doctype-system-identifier");
}
