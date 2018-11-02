#include <yourhtml/tokens/tag.h>

namespace yourhtml {

tag_t::tag_t(const pos_t &pos, bool closing):
  token_t(pos, closing ? token_t::END_TAG : token_t::START_TAG),
  self_closing(false) {}

tag_t::~tag_t() = default;

std::string tag_t::get_tag_name() const {
  return tag_name;
}

bool tag_t::is_self_closing() const {
  return self_closing;
}

void tag_t::set_self_closing(bool self_closing_) {
  self_closing = self_closing_;
}

void tag_t::append_attribute_name(const std::string &text) {
  std::get<0>(attributes.back()) += text;
}

void tag_t::append_attribute_name(char text) {
  std::get<0>(attributes.back()) += text;
}

void tag_t::append_attribute_value(const std::string &text) {
  std::get<1>(attributes.back()) += text;
}

void tag_t::append_attribute_value(char text) {
  std::get<1>(attributes.back()) += text;
}

void tag_t::start_new_attribute() {
  attributes.push_back(std::pair<std::string, std::string>({std::string(), std::string()}));
}

void tag_t::append_tag_name(const std::string &text) {
  tag_name += text;
}

void tag_t::append_tag_name(char text) {
  tag_name += text;
}

std::vector<std::pair<std::string, std::string>> tag_t::get_attributes() const {
  return attributes;
}

std::ostream &operator<<(std::ostream &strm, const tag_t &that) {
  strm << that.pos << "; " << tag_t::get_desc(that.kind);
  strm << "; <";
  if (that.kind == token_t::END_TAG) {
    strm << "/";
  }
  strm << that.get_tag_name();
  for (const auto &attr: that.get_attributes()) {
    strm << " " << std::get<0>(attr) << "=\"" << std::get<1>(attr) << "\"";
  }
  if (that.is_self_closing()) {
    strm << "/>";
  } else {
    strm << ">";
  }
  return strm;
}

std::ostream &operator<<(std::ostream &strm, const tag_t *that) {
  strm << *that;
  return strm;
}

}
