#include <test/utils/html5lib_tokenizer_test_helpers.h>

INSTANTIATE_TEST_CASE_P(
  html5lib_test_test1,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/test1.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_test2,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/test2.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_test3,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/test3.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_test4,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/test4.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_unicode_chars,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/unicodeChars.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_entities,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/entities.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_named_entities,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/namedEntities.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_numeric_entities,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/numericEntities.test"),
  html5lib_tokenizer_test_title_generator_t()
);

INSTANTIATE_TEST_CASE_P(
  html5lib_test_pending_spec_changes,
  html5lib_tokenizer_test_t,
  html5lib_test_params_in("test/html5lib-tests/tokenizer/pendingSpecChanges.test"),
  html5lib_tokenizer_test_title_generator_t()
);

// INSTANTIATE_TEST_CASE_P(
//   html5lib_test_domjs,
//   html5lib_tokenizer_test_t,
//   html5lib_test_params_in("test/html5lib-tests/tokenizer/domjs.test"),
//   html5lib_tokenizer_test_title_generator_t()
// );
