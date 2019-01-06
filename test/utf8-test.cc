#include <gtest/gtest.h>
#include <yourhtml/utf8.h>

using namespace yourhtml;


inline ::testing::AssertionResult is_invalid_sequence(utf8_decoder_t &decoder, const char *str) {
  try {
    for (size_t i = 0; i < sizeof(str); ++i) {
      decoder(static_cast<uint8_t>(str[i]));
    }
    return ::testing::AssertionFailure() << "expecting exception, but none thrown";
  } catch (const std::runtime_error &e) {
    return ::testing::AssertionSuccess() << "throws '" << e.what() << "'";
  }
}

inline ::testing::AssertionResult is_valid_sequence(utf8_decoder_t &decoder, const char *str, int codepoint) {
  try {
    for (size_t i = 0; i < sizeof(str); ++i) {
      auto result = decoder(static_cast<uint8_t>(str[i]));
      if (result != nullptr) {
        if (static_cast<int>(result[0]) != codepoint) {
          return ::testing::AssertionFailure() << "expected codepoint" << codepoint;
        } else {
          return ::testing::AssertionSuccess() << "return codepoint " << codepoint;
        }
      }
    }
    return ::testing::AssertionFailure() << "did not find any codepoints";
  } catch (const std::runtime_error &e) {
    return ::testing::AssertionFailure() << "unexpected exception '" << e.what() << "'";
  }
}

// Valid 2 Octet Sequence => "\xc3\xb1",
TEST(utf8_decoder, valid_2_octet_sequence) {
  utf8_decoder_t subject;
  EXPECT_TRUE(is_valid_sequence(subject, "\xc3\xb1", 0x00F1));
}

// Invalid 2 Octet Sequence => "\xc3\x28",
TEST(utf8_decoder, invalid_2_octet_sequence) {
  utf8_decoder_t subject;
  EXPECT_TRUE(is_invalid_sequence(subject, "\xc3\x28"));
}

// Valid 3 Octet Sequence => "\xe2\x82\xa1",
TEST(utf8_decoder, valid_3_octet_sequence) {
  utf8_decoder_t subject;
  EXPECT_TRUE(is_valid_sequence(subject, "\xe2\x82\xa1",  0x20A1));
}

// Valid 4 Octet Sequence => "\xf0\x90\x8c\xbc"
TEST(utf8_decoder, valid_4_octet_sequence) {
  utf8_decoder_t subject;
  EXPECT_TRUE(is_valid_sequence(subject, "\xf0\x90\x8c\xbc",  0x1033C));
}

// Valid 5 Octet Sequence (but not Unicode!) => "\xf8\xa1\xa1\xa1\xa1",
TEST(utf8_decoder, valid_5_octet_sequence_invalid_unicode) {
  utf8_decoder_t subject;
  EXPECT_TRUE(is_invalid_sequence(subject, "\xf8\xa1\xa1\xa1\xa1"));
}

// Valid ascii and unicode together -> <!doc\uD83F\uDFFF
TEST(utf8_decoder_t, typical_usage) {
  utf8_decoder_t subject;
  const char *str = "<!doc\xed\xa0\xbf\xed\xbf\xbf";
  // <
  {
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 60);
    ++str;
  }
  // ! 33
  {
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 33);
    ++str;
  }
  // d 100
  {
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 100);
    ++str;
  }
  // o 111
  {
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 111);
    ++str;
  }
  // c 99
  {
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 99);
    ++str;
  }
  // 0xD83F
  {
    EXPECT_TRUE(subject(static_cast<uint8_t>(*str)) == nullptr);
    ++str;
    EXPECT_TRUE(subject(static_cast<uint8_t>(*str)) == nullptr);
    ++str;
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 0xD83F);
    ++str;
  }
  // 0xDFFF
  {
    EXPECT_TRUE(subject(static_cast<uint8_t>(*str)) == nullptr);
    ++str;
    EXPECT_TRUE(subject(static_cast<uint8_t>(*str)) == nullptr);
    ++str;
    const char32_t *lt = subject(static_cast<uint8_t>(*str));
    EXPECT_TRUE(lt != nullptr);
    EXPECT_EQ(static_cast<int>(lt[0]), 0xDFFF);
  }
}
