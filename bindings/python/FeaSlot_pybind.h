/// file: FeaSlot_pybind.h


#ifndef FEATHER_FEASLOT_PYBIND_H_
#define FEATHER_FEASLOT_PYBIND_H_


#include <pybind11/pybind11.h>
#include "feather/FeaSlot.h"

namespace py = pybind11;


namespace feather {


void FeaSlot_pybind(py::module &m);


} // namespace feather


#endif
