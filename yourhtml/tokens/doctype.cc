#include <yourhtml/tokens/doctype.h>
#include <iostream>

namespace yourhtml {

doctype_t::doctype_t(pos_t pos):
  token_t(pos, token_t::DOCTYPE),
  force_quirks(false),
  public_identifier_missing(true),
  doctype_name_missing(true),
  system_identifier_missing(true) {}

doctype_t::doctype_t():
  token_t(token_t::DOCTYPE),
  force_quirks(false),
  public_identifier_missing(true),
  doctype_name_missing(true),
  system_identifier_missing(true) {}


doctype_t::~doctype_t() = default;

std::string doctype_t::get_doctype_name() const {
  if (doctype_name_missing) {
    throw std::runtime_error("cannot get missing doctype name");
  }
  return doctype_name;
}

std::string doctype_t::get_public_identifier() const {
  if (public_identifier_missing) {
    throw std::runtime_error("cannot get missing public identifier name");
  }
  return public_identifier;
}

std::string doctype_t::get_system_identifier() const {
  if (system_identifier_missing) {
    throw std::runtime_error("cannot get missing public identifier name");
  }
  return system_identifier;
}

void doctype_t::append_doctype_name(const std::string &text) {
  doctype_name += text;
  doctype_name_missing = false;
}

void doctype_t::append_doctype_name(char text) {
  doctype_name += text;
  doctype_name_missing = false;
}

void doctype_t::append_public_identifier(const std::string &text) {
  public_identifier += text;
  public_identifier_missing = false;
}

void doctype_t::append_public_identifier(char text) {
  public_identifier += text;
  public_identifier_missing = false;
}

void doctype_t::append_system_identifier(const std::string &text) {
  system_identifier += text;
  system_identifier_missing = false;
}

void doctype_t::append_system_identifier(char text) {
  system_identifier += text;
  system_identifier_missing = false;
}

void doctype_t::set_doctype_name(const std::string &doctype_name_) {
  doctype_name = doctype_name_;
  doctype_name_missing = false;
}

void doctype_t::set_public_identifier(const std::string &public_identifier_) {
  public_identifier = public_identifier_;
  public_identifier_missing = false;
}

void doctype_t::set_system_identifier(const std::string &system_identifier_) {
  system_identifier = system_identifier_;
  system_identifier_missing = false;
}

void doctype_t::set_force_quirks(bool force_quirks_) {
  force_quirks = force_quirks_;
}

bool doctype_t::is_missing_public_identifier() const {
  return public_identifier_missing;
}

bool doctype_t::is_missing_doctype_name() const {
  return doctype_name_missing;
}

bool doctype_t::is_missing_system_identifier() const {
  return system_identifier_missing;
}

std::ostream &operator<<(std::ostream &strm, const doctype_t &that) {
  strm << that.pos << "; " << doctype_t::get_desc(that.kind);
  if (!that.is_missing_doctype_name()) {
    strm << "; name=\"" << that.get_doctype_name() << "\"";
  }
  if (!that.is_missing_public_identifier()) {
    strm << "; public_identifier=\"" << that.get_public_identifier() << "\"";
  }
  if (!that.is_missing_system_identifier()) {
    strm << "; system_identifier=\"" << that.get_system_identifier() << "\"";
  }
  if (that.is_forcing_quirks()) {
    strm << "; force quirks = true";
  } else {
    strm << "; force quirks = false";
  }
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const doctype_t *that) {
  strm << *that;
  return strm;
}

bool doctype_t::is_forcing_quirks() const {
  return force_quirks;
}

bool operator==(const doctype_t &lhs, const doctype_t &rhs) {
  if (
    lhs.is_forcing_quirks() != rhs.is_forcing_quirks() ||
    lhs.is_missing_system_identifier() != rhs.is_missing_system_identifier() ||
    lhs.is_missing_doctype_name() != rhs.is_missing_doctype_name() ||
    lhs.is_missing_public_identifier() != rhs.is_missing_public_identifier() ||
    (
      !lhs.is_missing_system_identifier() &&
      lhs.get_system_identifier() != rhs.get_system_identifier()
    ) ||
    (
      !lhs.is_missing_public_identifier() &&
      lhs.get_public_identifier() != rhs.get_public_identifier()
    ) ||
    (
      !lhs.is_missing_doctype_name() &&
      lhs.get_doctype_name() != rhs.get_doctype_name()
    )
  ) {
    return false;
  }
  return true;
}

}
