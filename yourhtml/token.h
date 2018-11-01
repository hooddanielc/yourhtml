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

  token_t(const token_t&) = default;

  token_t(token_t&&) = default;

  token_t& operator=(const token_t&) = default;

  token_t& operator=(token_t&&) = default;

  static std::string get_desc(kind_t kind);

  kind_t get_kind() const;

  /* Returns kind as a string */
  std::string get_description() const;

  pos_t get_pos() const;

  static std::shared_ptr<token_t> make(kind_t kind);

  static std::shared_ptr<token_t> make(const pos_t &pos, kind_t kind);

  virtual ~token_t();

  template <typename as_t>
  as_t get_as() {
    return *(dynamic_cast<as_t*>(this));
  }

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

};  // token_t

}   // yourcss
