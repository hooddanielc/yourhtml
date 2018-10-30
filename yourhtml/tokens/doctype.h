#pragma once
#include <yourhtml/token.h>

namespace yourhtml {

class doctype_t: public token_t {

public:

  doctype_t(pos_t);

  std::string get_doctype_name();

  std::string get_public_identifier();

  std::string get_system_identifier();

  bool set_force_quirks(bool);

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

  bool is_missing_public_identifier();

  bool is_missing_doctype_name();

  bool is_missing_system_identifier();

  virtual ~doctype_t();

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
