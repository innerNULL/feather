/// file: pymodule.cpp


#include "feather/FeaHash.h"
#include "feather/FeaSlot.h"
#include "feather/LibsvmExtractor.h"


namespace feather {


PYBIND11_MODULE(pyfeather, m) {
  FeaSlot_pybind(m);
  FeaHash_pybind(m);
  LibsvmExtractor_pybind(m);
}


} // namespace feather
