#include <yourhtml/tokens/tag.h>

namespace yourhtml {

tag_t::tag_t(bool closing):
  token_t(closing ? token_t::END_TAG : token_t::START_TAG),
  self_closing(false) {}

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

size_t tag_t::get_num_attributes() const {
  return attributes.size();
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
  std::cout << "APPEND BEFORE : " << std::get<1>(attributes.back()).size() << std::endl;
  std::get<1>(attributes.back()) += text;
  std::cout << "APPEND AFTER : " << std::get<1>(attributes.back()).size() << std::endl;
}

void tag_t::start_new_attribute() {
  attributes.push_back(std::pair<std::string, std::string>({std::string(), std::string()}));
}

int tag_t::remove_duplicate_attributes() {
  int num_removed = 0;
  std::vector<std::pair<std::string, std::string>> new_attributes;
  std::unordered_map<std::string, bool> taboo;
  for (auto item: get_attributes()) {
    auto key = std::get<0>(item);
    if (taboo.find(key) == taboo.end()) {
      taboo[key] = true;
      new_attributes.push_back(item);
    } else {
      num_removed += 1;
    }
  }
  attributes = new_attributes;
  return num_removed;
}

void tag_t::append_tag_name(const std::string &text) {
  tag_name += text;
}

void tag_t::append_tag_name(char text) {
  tag_name += text;
}

std::unordered_map<std::string, std::string> tag_t::get_attribute_map() const {
  std::unordered_map<std::string, std::string> result;
  for (auto item: get_attributes()) {
    result[std::get<0>(item)] = std::get<1>(item);
  }
  return result;
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

bool operator==(const tag_t &lhs, const tag_t &rhs) {
  if (
    lhs.get_kind() != rhs.get_kind() ||
    lhs.is_self_closing() != rhs.is_self_closing() ||
    lhs.get_attributes().size() != rhs.get_attributes().size()
  ) {
    return false;
  }

  if (lhs.get_attributes() != rhs.get_attributes()) {
    return false;
  }

  return true;
}

}
