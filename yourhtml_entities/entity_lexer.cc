#include <yourhtml_entities/entity_lexer.h>

namespace yourhtml {

flex_lexer_istream_t::flex_lexer_istream_t(char *s, char *end) {
  setg(s, s, end);
}

// std::streamsize flex_lexer_istream_t::xsgetn(char *s, std::streamsize n) {
//   std::cout << "GET SEQ OF CHARS: " << n << std::endl;
//   auto res = std::streambuf::xsgetn(s, n);
//   std::cout << "RETURNED RESULT ? " << res << std::endl;
//   return res;
// }

flex_lexer_istream_t::~flex_lexer_istream_t() = default;

int match_entity(char *start, char *end, std::ostringstream &os) {
  yyFlexLexer lexer;
  flex_lexer_istream_t isb(start, end);
  std::istream is(&isb);
  return lexer.FlexLexer::yylex(is, os);
}

}

   // int main(int, char**) {
   //   using namespace yourhtml;
   //   std::ostringstream os;
   //   const char *src = "&notin;try it";
   //   //const char *src = "This will fail. &notin;try it";
   //   const char *start = src;
   //   const char *end = start + 8;
   
   //   auto match = match_entity(
   //     const_cast<char*>(start),
   //     const_cast<char*>(end),
   //     os
   //   );
   
   //   std::cout << "INDEX: " << match << std::endl;
   //   std::cout << "UTF8 STR: '" << os.str() << "'" << std::endl;
   //   std::cout << "PARSED NAME: " << entity_names[match] << std::endl;
   //   return 0;
   // }