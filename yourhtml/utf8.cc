#include <yourhtml/utf8.h>

namespace yourhtml {

bool utf8_decoder_t::is_at_start() const noexcept {
  return state == state_t::start;
}

bool utf8_decoder_t::is_start_byte(uint8_t byte) noexcept {
  return
    ((byte & 0xE0) == 0xC0) ||
    ((byte & 0xF0) == 0xE0) ||
    ((byte & 0xF8) == 0xF0) ||
    ((byte & 0xFC) == 0xF8) ||
    ((byte & 0xFE) == 0xFC);
}

utf8_decoder_t::utf8_decoder_t() noexcept:
  state(state_t::start),
  current_size(0),
  final_size(1) {
  result[1] = U'\0';
}

void utf8_decoder_t::print_state(std::ostream &ss) const noexcept {
  switch (state) {
    case state_t::start: {
      ss << "state: 'start', size: '" << current_size << "'";
      break;
    }
    case state_t::follow_on: {
      ss << "state: 'follow_on', size: '" << current_size << "' final_size: '" << final_size << "'";
      break;
    }
    case state_t::panic: {
      ss << "state: 'panic', size: '" << current_size << "'";
      break;
    }
  }
}

const char32_t *utf8_decoder_t::operator()(uint8_t byte) {
  for (;;) {
    switch (state) {
      case state_t::start: {
        // If the byte is plain ASCII, return it as a wide char.
        if (byte <= 0x7F) {
          *result = byte;
          return result;
        }
        // This is the start of a multi-byte sequence.
        if ((byte & 0xE0) == 0xC0) {
          accum = byte & 31;
          final_size = 2;
        } else if ((byte & 0xF0) == 0xE0) {
          accum = byte & 15;
          final_size = 3;
        } else if ((byte & 0xF8) == 0xF0) {
          accum = byte & 7;
          final_size = 4;
        } else if ((byte & 0xFC) == 0xF8) {
          accum = byte & 3;
          final_size = 5;
        } else if ((byte & 0xFE) == 0xFC) {
          accum = byte & 1;
          final_size = 6;
        } else {
          state = state_t::panic;
          throw std::runtime_error { "bad UTF-8 start byte" };
        }
        // We have accumulated one byte of the sequence and
        // will change mode to accept the rest.
        state = state_t::follow_on;
        current_size = 1;
        return nullptr;
      }
      case state_t::follow_on: {
        // If this isn't a follow-on byte, we've hit bad encoding.
        if ((byte & 0xC0) != 0x80) {
          state = state_t::panic;
          throw std::runtime_error { "bad UTF-8 follow-on byte" };
        }
        // Accumulate the bits.
        accum = (accum << 6) | (byte & 0x3F);
        ++current_size;
        // If we're expecting more follow-on bytes, return empty-handed.
        if (current_size != final_size) {
          return nullptr;
        }
        // Resume the start state and validate the result.
        state = state_t::start;
        if (accum > 0x10FFFF) {
          throw std::runtime_error { "out of UTF-8 range" };
        }
        // Return the result.
        *result = accum;
        return result;
      }
      case state_t::panic: {
        // If this is a follow-on byte, skip it.
        if ((byte & 0xC0) == 0x80) {
          return nullptr;
        }
        // We've reached a start byte, so stop panicking and
        // start decoding.
        state = state_t::start;
        break;
      }
    }  // switch
  }  // for
}

void utf8_decoder_t::finish() {
  switch (std::exchange(state, state_t::start)) {
    case state_t::start:
    case state_t::panic: {
      break;
    }
    case state_t::follow_on: {
      throw std::runtime_error { "expected UTF-8 follow-on byte(s)" };
    }
  }  // switch
}

std::ostream &operator<<(std::ostream &strm, const utf8_decoder_t &that) {
  that.print_state(strm);
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const utf8_decoder_t *that) {
  that->print_state(strm);
  return strm;
}


}  // tarp::txt::utf8
