/// file: FeaHash_pybind.h


#ifndef FEATHER_FEAHASH_PYBIND_H_
#define FEATHER_FEAHASH_PYBIND_H_


#include <pybind11/pybind11.h> 

namespace py = pybind11;


namespace feather {


void FeaHash_pybind(py::module& m);


} // namespace feather


#endif 
