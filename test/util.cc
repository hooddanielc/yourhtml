#include "util.h"

namespace yourhtml {

type_accumlator_t::~type_accumlator_t() = default;

std::string read_file(const std::string &relative_path) {
  auto abs_path = get_ib_project_path(relative_path).string();

  std::cout << "reading file " << abs_path << std::endl;

  std::ifstream t(abs_path);
  std::string str;

  t.seekg(0, std::ios::end);   
  str.reserve(static_cast<unsigned long>(t.tellg()));
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)),
              std::istreambuf_iterator<char>());
  return str;
}

void lexer_with_errors_t::on_parse_error(const lexer_error_t &error) {
  error_types.push_back(error.get_type());
}

testfs::path get_ib_src_path() {
    #define STRINGIFY(x) #x
    #define TOSTRING(x) STRINGIFY(x)
    testfs::path path(TOSTRING(IB_SRC_ROOT));
    #undef TOSTRING
    #undef STRINGIFY
    return path;
}

testfs::path get_ib_project_path(const std::string &relative) {
    #define STRINGIFY(x) #x
    #define TOSTRING(x) STRINGIFY(x)
    testfs::path path(TOSTRING(IB_SRC_ROOT));
    path /= relative;
    #undef TOSTRING
    #undef STRINGIFY
    return path;
}

}
