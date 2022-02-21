/// file: FeaSlot_pybind.cpp


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../FeaSlot_pybind.h"

namespace py = pybind11;


namespace feather {


void FeaSlot_pybind(py::module &m) {
  py::class_<FeaSlot>(m, "FeaSlot")
      .def(py::init<const std::string&, const int32_t, const int32_t, const int8_t>())
      .def("Info", &FeaSlot::Info)
      .def("ValRegister", &FeaSlot::ValRegister)
      .def("GetBucketID", 
          static_cast< std::vector<int32_t>  (FeaSlot::*)(
            const std::string&) >(&FeaSlot::GetBucketID));
}


} // namespace feather
