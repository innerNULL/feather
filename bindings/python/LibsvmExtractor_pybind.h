/// file: LibsvmExtractor_pybind.h


#ifndef FEATHER_LIBSVMEXTRACTOR_PYBIND_H_
#define FEATHER_LIBSVMEXTRACTOR_PYBIND_H_


#include <pybind11/pybind11.h>

namespace py = pybind11;


namespace feather {


void LibsvmExtractor_pybind(py::module &m); 


} // namespace feather


#endif
