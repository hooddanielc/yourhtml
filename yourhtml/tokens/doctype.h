#pragma once
#include <yourhtml/token.h>

namespace yourhtml {

class doctype_t: public token_t {

public:

  doctype_t(pos_t);

  doctype_t(const doctype_t&) = default;

  doctype_t(doctype_t&&) = default;

  doctype_t& operator=(const doctype_t&) = default;

  doctype_t& operator=(doctype_t&&) = default;

  std::string get_doctype_name() const;

  std::string get_public_identifier() const;

  std::string get_system_identifier() const;

  void set_force_quirks(bool);

  void set_doctype_name(const std::string &doctype_name_);

  void append_doctype_name(const std::string &text);

  void append_doctype_name(char c);

  void set_public_identifier(const std::string &public_identifier_);

  void append_public_identifier(const std::string &text);

  void append_public_identifier(char c);

  void set_system_identifier(const std::string &system_identifier_);

  void append_system_identifier(const std::string &text);

  void append_system_identifier(char c);

  bool is_forcing_quirks();

  bool is_missing_public_identifier() const;

  bool is_missing_doctype_name() const;

  bool is_missing_system_identifier() const;

  virtual ~doctype_t();

  friend std::ostream &operator<<(std::ostream &strm, const doctype_t &that);

  friend std::ostream &operator<<(std::ostream &strm, const doctype_t *that);

private:

  bool force_quirks;

  bool public_identifier_missing;

  bool doctype_name_missing;

  bool system_identifier_missing;

  std::string doctype_name;

  std::string public_identifier;

  std::string system_identifier;

};

}
