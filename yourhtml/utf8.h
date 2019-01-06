#pragma once

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <iostream>

namespace yourhtml {

// A helper object for decoding wide characters from UTF-8.
class utf8_decoder_t final {

public:

  utf8_decoder_t(const utf8_decoder_t &) = default;

  utf8_decoder_t &operator=(const utf8_decoder_t &) = default;

  utf8_decoder_t(utf8_decoder_t &&) = default;

  utf8_decoder_t &operator=(utf8_decoder_t &&) = default;

  utf8_decoder_t() noexcept;

  // Pass bytes to this function one at at time.  When a UTF-8
  // character is formed, the function returns a pointer to it.
  // While the character is still only partly formed, this function
  // returns a null pointer.
  const char32_t *operator()(uint8_t byte);

  // Call this function when there is no more input.  If the decoder
  // is expecting a starting byte, this does nothing; otherwise, this
  // throws a final exception.  The decoder is left in a state where
  // it can be used again.
  void finish();

  // True if the decoder is currently expecting a starting byte.
  bool is_at_start() const noexcept;

  // True if the given byte would mark the start of a multi-byte sequence.
  static bool is_start_byte(uint8_t byte) noexcept;

  // Print a message to desired ostream
  void print_state(std::ostream &ostrm = std::cout) const noexcept;

  friend std::ostream &operator<<(std::ostream &strm, const utf8_decoder_t &that);

  friend std::ostream &operator<<(std::ostream &strm, const utf8_decoder_t *that);

private:

  // The state of this mini-parser.
  enum class state_t {

    // Expecting a start byte.
    start,

    // Expecting a follow-on byte.
    follow_on,

    // Recovering from an error and looking
    // for the next start byte.
    panic

  } state;

  // The number of bytes we have currently accumulated.
  size_t current_size;

  // The number of bytes we need to accumulate to decode the character.
  size_t final_size;

  // The accumulated bits of the character.
  char32_t accum;

  // The buffer in which we store the final character and a null terminator.
  char32_t result[2];

};  // utf8_decoder_t

}  // yourhtml
