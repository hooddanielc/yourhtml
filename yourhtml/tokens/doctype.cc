#include <yourhtml/tokens/doctype.h>

namespace yourhtml {

doctype_t::doctype_t(pos_t pos):
  token_t(pos, token_t::DOCTYPE),
  force_quirks(false),
  public_identifier_missing(true),
  doctype_name_missing(true),
  system_identifier_missing(true) {}


doctype_t::~doctype_t() = default;

std::string doctype_t::get_doctype_name() {
  if (doctype_name_missing) {
    throw std::runtime_error("cannot get missing doctype name");
  }
  return doctype_name;
}

std::string doctype_t::get_public_identifier() {
  if (public_identifier_missing) {
    throw std::runtime_error("cannot get missing public identifier name");
  }
  return public_identifier;
}

std::string doctype_t::get_system_identifier() {
  if (system_identifier_missing) {
    throw std::runtime_error("cannot get missing public identifier name");
  }
  return system_identifier;
}

void doctype_t::append_doctype_name(const std::string &text) {
  doctype_name += text;
}

void doctype_t::append_doctype_name(char text) {
  doctype_name += text;
}

void doctype_t::append_public_identifier(const std::string &text) {
  public_identifier += text;
}

void doctype_t::append_public_identifier(char text) {
  public_identifier += text;
}

void doctype_t::append_system_identifier(const std::string &text) {
  system_identifier += text;
}

void doctype_t::append_system_identifier(char text) {
  system_identifier += text;
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

bool doctype_t::set_force_quirks(bool force_quirks_) {
  force_quirks = force_quirks_;
}

bool doctype_t::is_missing_public_identifier() {
  return public_identifier_missing;
}

bool doctype_t::is_missing_doctype_name() {
  return doctype_name_missing;
}

bool doctype_t::is_missing_system_identifier() {
  return system_identifier_missing;
}

}
