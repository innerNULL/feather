/// file: FeaHash_pybind.cpp


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "feather/FeaHash.h"
#include "bindings/python/FeaHash_pybind.h"

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
            const std::string&, const std::string&, uint16_t)>(&FeaHash::GetFeaHash), 
          py::arg("fea_name"), py::arg("fea_value"), py::arg("hash_type") = 0)
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const int32_t, uint16_t)>(&FeaHash::GetFeaHash), 
          py::arg("fea_name"), py::arg("fea_value"), py::arg("hash_type") = 0)
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const float, uint16_t)>(&FeaHash::GetFeaHash), 
          py::arg("fea_name"), py::arg("fea_value"), py::arg("hash_type") = 0)
      .def("GetFeaHash", 
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::vector<float>&, uint16_t)>(&FeaHash::GetFeaHash), 
          py::arg("fea_name"), py::arg("fea_value"), py::arg("hash_type") = 0)
      .def("GetFeaHash",
          static_cast<std::vector<int64_t> (FeaHash::*)(
            const std::string&, const std::vector<std::string>&, uint16_t)>(&FeaHash::GetFeaHash), 
          py::arg("fea_name"), py::arg("fea_value"), py::arg("hash_type") = 0)
      .def("GetFeaHash",
          static_cast<std::vector<int64_t> (FeaHash::*)( 
            const std::string&, const std::vector<int32_t>&, uint16_t)>(&FeaHash::GetFeaHash), 
          py::arg("fea_name"), py::arg("fea_value"), py::arg("hash_type") = 0); 
}


} // namespace feather
