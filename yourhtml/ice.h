#pragma once

#include "pos.h"
#include "error.h"

namespace yourhtml {

/* The dreaded internal compiler error. */
class ice_t: public error_t {

public:

  /* Report the file and line at which we iced. */
  ice_t(const pos_t &pos, const char *file, int line_number);

  virtual ~ice_t() override;

};  // ice_t

}  // biglr
