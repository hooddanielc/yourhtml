#pragma once

#include <sstream>
#include <iostream>
#include <yourhtml_entities/entity_names.h>
#include <yourhtml_entities/entity_lexer_flex.h>

namespace yourhtml {

int match_entity(char *start, char *end, std::ostringstream &os);

/* A stream but used in std::istringstream class
   that does not copy an internal buffer */
struct flex_lexer_istream_t: public std::streambuf {
  flex_lexer_istream_t(char *s, char *end);
  // virtual std::streamsize xsgetn(char *s, std::streamsize n);
  virtual ~flex_lexer_istream_t();
};

}
