/// file: pymodule.cpp


#include "bindings/python/FeaHash_pybind.h"
#include "bindings/python/FeaSlot_pybind.h"
#include "bindings/python/LibsvmExtractor_pybind.h"


namespace feather {


PYBIND11_MODULE(pyfeather, m) {
  FeaSlot_pybind(m);
  FeaHash_pybind(m);
  LibsvmExtractor_pybind(m);
}


} // namespace feather
