#include <yourhtml/tokens/tag.h>

namespace yourhtml {

tag_t::tag_t(const pos_t &pos, bool closing):
  token_t(pos, closing ? token_t::END_TAG : token_t::START_TAG),
  self_closing(false) {}

tag_t::~tag_t() = default;

std::string tag_t::get_tag_name() {
  return tag_name;
}

bool tag_t::is_self_closing() {
  return self_closing;
}

void tag_t::set_self_closing(bool self_closing_) {
  self_closing = self_closing_;
}

void tag_t::append_attribute_name(const std::string &text) {
  std::get<0>(attributes.back()) += text;
}

void tag_t::append_attribue_name(char text) {
  std::get<0>(attributes.back()) += text;
}

void tag_t::append_attribue_value(const std::string &text) {
  std::get<1>(attributes.back()) += text;
}

void tag_t::append_attribue_value(char text) {
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

}
