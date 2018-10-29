#pragma once

#include <ostream>
#include <string>
#include <utility>
#include <map>
#include <memory>
#include "pos.h"

namespace yourhtml {

class token_t {

public:

  // The output of the tokenization step is a series of zero or more of the
  // following tokens: DOCTYPE, start tag, end tag, comment, character, 
  // end-of-file.
  enum kind_t {
    DOCTYPE,
    START_TAG,
    END_TAG,
    COMMENT,
    CHARACTER,
    END_OF_FILE,
  };  // kind_t

  /* Cache the kind. */
  token_t(kind_t kind);

  /* Cache the position and kind and set the text to the empty string. */
  token_t(const pos_t &pos, kind_t kind);

  /* Cache the position and kind and the text. */
  token_t(const pos_t &pos, kind_t kind, std::string &&text);

  static std::string get_desc(kind_t kind);

  kind_t get_kind() const;

  std::string get_text() const;

  /* Returns kind as a string */
  std::string get_description() const;

  /* Returns the tokens name */
  std::string get_name() const;

  void set_name(const std::string &);

  void append_name(const std::string &);

  pos_t get_pos() const;

  static std::shared_ptr<token_t> make(kind_t kind);

  static std::shared_ptr<token_t> make(const pos_t &pos, kind_t kind);

  static std::shared_ptr<token_t> make(const pos_t &pos, kind_t kind, std::string &&text);

  virtual ~token_t();

protected:

  /* Writes a human-readable dump of the token. */
  friend std::ostream &operator<<(std::ostream &strm, const token_t &that);

  friend std::ostream &operator<<(std::ostream &strm, const token_t *that);

  /* See accessors. */
  pos_t pos;

  /* See accessor. */
  kind_t kind;

  /* See accessor. */
  std::string text;

  /* See accessor. */
  std::string name;

};  // token_t

}   // yourcss
