// inclusion guard
#ifndef _CORRELATOR_H_
#define _CORRELATOR_H_

#include <hoomd/Analyzer.h>

// add pybind??

#ifndef NVCC
#include <hoomd/extern/pybind/include/pybind11/pybind11.h>
#endif

class Correlator : public Analyzer
  {
  public:
    //! Constructor
    Correlator(std::shared_ptr<Logger>m_logger): 

    // don't think we need to advance timestep since it's an analyzer
  }
//! Export the Correlator class to python
void export_Correlator(pybind11::module& m);

// NO ENABLE_CUDA
#endif
