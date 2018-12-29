#include "util.h"

namespace yourhtml {

type_accumlator_t::~type_accumlator_t() = default;

#define IB_SRC_ROOT_TO_STRING(s) #s

#define IB_SRC_ROOT_STRING(s) IB_SRC_ROOT_TO_STRING(s)

std::string read_file(const std::string &relative_path) {
  std::string abs_path(IB_SRC_ROOT_STRING(IB_SRC_ROOT));
  abs_path += "/" + relative_path;

  std::cout << "reading file " << abs_path << std::endl;

  std::ifstream t(abs_path);
  std::string str;

  t.seekg(0, std::ios::end);   
  str.reserve(t.tellg());
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)),
              std::istreambuf_iterator<char>());
  return str;
}

void lexer_with_errors_t::on_parse_error(const lexer_error_t &error) {
  error_types.push_back(error.get_type());
}

}
