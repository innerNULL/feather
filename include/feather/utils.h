/// file: utils.h


#ifndef FEATHER_UTILS_H_
#define FEATHER_UTILS_H_


#include <string>


namespace feather {


template <typename NUM_TYPE=int32_t>
std::string num2str_code(
    const NUM_TYPE num, const int32_t code_digits) {
  std::string num_str = std::to_string(num);
  if (num_str.size() == code_digits) {
    return num_str;
  } else if (num_str.size() < code_digits) {
    std::string concating_zero(code_digits - num_str.size(), '0');
    return concating_zero + num_str;
  } else {
    throw "num str shouldn't has larget length than `code_digits`.";
  }
}


} // namespace feather


#endif
