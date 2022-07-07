
#include <numeric>
#include <vector>

#include <utf8.h>

#include <tensor.hpp>

auto unary_iota(tensor<int> array) -> tensor<int> {
  if (array.rank() != 0) {
    // rank not supported with std::expected
  }
  auto const n = array.data().front();
  auto vec = std::vector<int32_t>(n);
  std::iota(vec.begin(), vec.end(), 1);
  return tensor{vec};
}