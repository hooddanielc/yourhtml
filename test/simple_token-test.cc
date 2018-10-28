#include <lick/lick.h>
#include <yourhtml/lexer.h>
#include <yourhtml/token.h>

using namespace yourhtml;

FIXTURE(construction) {
  const char *src = R"(
    #
  )";
  auto tokens = lexer_t(src);
}
