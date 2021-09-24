/// file: FeaHash_pybind.cpp


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "feather/FeaHash.h"
#include "feather/pybind/FeaHash_pybind.h"

namespace py = pybind11;


namespace feather {


void FeaHash_pybind(py::module& m) {
  py::class_<FeaHash>(m, "FeaHash")
      .def(py::init<>())
      .def(py::init<const std::string&>())
      .def("SlotRegister", &FeaHash::SlotRegister)
      .def("FeaHash2FeaName", &FeaHash::FeaHash2FeaName)
      .def("SchemaLineRegister", &FeaHash::SchemaLineRegister)
      .def("Hash2IndexDictBuild", &FeaHash::Hash2IndexDictBuild)
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::string&)>(&FeaHash::GetFeaHash))
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const int32_t)>(&FeaHash::GetFeaHash))
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const float)>(&FeaHash::GetFeaHash))
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::vector<float>&)>(&FeaHash::GetFeaHash))
      .def("GetFeaHash",
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::vector<std::string>&)>(&FeaHash::GetFeaHash))
      .def("GetFeaHash",
          static_cast<std::vector<int64_t> (FeaHash::*)( 
            const std::string&, const std::vector<int32_t>&)>(&FeaHash::GetFeaHash));
}


} // namespace feather
