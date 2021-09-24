/// file: LibsvmExtractor_pybind.cpp


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "feather/LibsvmExtractor.h"
#include "feather/pybind/LibsvmExtractor_pybind.h"

namespace py = pybind11;


namespace feather {


void LibsvmExtractor_pybind(py::module &m) {
  py::class_<LibsvmExtractor>(m, "LibsvmExtractor")
      .def(py::init<>())
      .def(py::init<const std::string&, const std::string&, const bool>())
      .def("Extract", 
          static_cast<std::string (LibsvmExtractor::*)(const std::string&, const bool)>(
            &LibsvmExtractor::Extract))
      .def("BatchExtract", 
          static_cast<
              std::vector<std::string> (LibsvmExtractor::*)(const std::vector<std::string>&, const bool)
          >(
              &LibsvmExtractor::BatchExtract));
}


} // namespace feather
