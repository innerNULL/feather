/// file: pymodule.cpp


#include "../FeaHash_pybind.h"
#include "../FeaSlot_pybind.h"
#include "../LibsvmExtractor_pybind.h"


namespace feather {


PYBIND11_MODULE(pyfeather, m) {
  FeaSlot_pybind(m);
  FeaHash_pybind(m);
  LibsvmExtractor_pybind(m);
}


} // namespace feather
