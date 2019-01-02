#pragma once

#include <sstream>
#include <iostream>
#include <yourhtml_entities/entity_lexer_flex.h>

namespace yourhtml {

/*
   Efficient method for matching an html entity
   against an existing buffer. Example usage:
   
   int main(int, char**) {
     const char *src = "&notin;try it";
     //const char *src = "This will fail. &notin;try it";
     const char *start = src;
     const char *end = start + 10;
   
     auto match = match_entity(
       const_cast<char*>(start),
       const_cast<char*>(end)
     );
   
     std::cout << "SIZE: " << match.size() << std::endl;
     std::cout << "MATCH: '" << match << "'" << std::endl;
     return 0;
   }
*/
std::string match_entity(char *start, char *end);

/* A stream but used in std::istringstream class
   that does not copy an internal buffer */
struct flex_lexer_istream_t: public std::streambuf {
  flex_lexer_istream_t(char *s, char *end);
  virtual ~flex_lexer_istream_t();
};

}
