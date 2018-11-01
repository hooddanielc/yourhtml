#pragma once
#include <vector>
#include <utility>
#include <yourhtml/token.h>

namespace yourhtml {

class tag_t: public token_t {

public:

  tag_t(const pos_t &pos, bool closing = false);

  tag_t(const tag_t&) = default;

  tag_t(tag_t&&) = default;

  tag_t& operator=(const tag_t&) = default;

  tag_t& operator=(tag_t&&) = default;

  std::string get_tag_name() const;

  std::vector<std::pair<std::string, std::string>> get_attributes() const;

  bool is_self_closing() const;

  void set_self_closing(bool self_closing_);

  void append_attribute_name(const std::string &text);

  void append_attribue_name(char text);

  void append_attribue_value(const std::string &text);

  void append_attribue_value(char text);

  void append_tag_name(const std::string &text);

  void append_tag_name(char text);

  void start_new_attribute();

  virtual ~tag_t();

  friend std::ostream &operator<<(std::ostream &strm, const tag_t &that);

  friend std::ostream &operator<<(std::ostream &strm, const tag_t *that);

private:

  std::string tag_name;

  bool self_closing;

  std::vector<std::pair<std::string, std::string>> attributes;

};

}