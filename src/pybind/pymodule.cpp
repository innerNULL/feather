/// file: pymodule.cpp


#include "feather/pybind/FeaHash_pybind.h"
#include "feather/pybind/FeaSlot_pybind.h"
#include "feather/pybind/LibsvmExtractor_pybind.h"


namespace feather {


PYBIND11_MODULE(pyfeather, m) {
  FeaSlot_pybind(m);
  FeaHash_pybind(m);
  LibsvmExtractor_pybind(m);
}


} // namespace feather
