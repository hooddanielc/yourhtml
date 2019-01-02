#include <yourhtml_entities/entity_lexer.h>

namespace yourhtml {

flex_lexer_istream_t::flex_lexer_istream_t(char *s, char *end) {
  setg(s, s, end);
}

flex_lexer_istream_t::~flex_lexer_istream_t() = default;

std::string match_entity(char *start, char *end) {
  yyFlexLexer lexer;
  flex_lexer_istream_t isb(start, end);
  std::istream is(&isb);
  std::ostringstream os;
  lexer.FlexLexer::yylex(is, os);
  return os.str();
}

}
