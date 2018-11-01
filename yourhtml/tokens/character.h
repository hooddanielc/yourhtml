#pragma once
#include <yourhtml/token.h>

namespace yourhtml {

class character_t: public token_t {

public:

  character_t(const pos_t &pos, char c);

  character_t(const pos_t &pos, const char *data_);

  character_t(const character_t&) = default;
  character_t(character_t&&) = default;
  character_t& operator=(const character_t&) = default;
  character_t& operator=(character_t&&) = default;

  virtual ~character_t();

  const char *get_data() const;

  friend std::ostream &operator<<(std::ostream &strm, const character_t &that);

  friend std::ostream &operator<<(std::ostream &strm, const character_t *that);

  bool is_space() const;

private:

  mutable const char *data;

};

}
