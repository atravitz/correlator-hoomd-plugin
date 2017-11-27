// inclusion guard
#ifndef _CORRELATOR_H_
#define _CORRELATOR_H_

#include <hoomd/Analyzer.h>
#include <hoomd/Logger.h>



#ifndef NVCC
#include <hoomd/extern/pybind/include/pybind11/pybind11.h>
#endif

class Correlator : public Analyzer
  {
  public:
    //! Constructor
    Correlator(std::shared_ptr<Logger> m_logger, std::shared_ptr<SystemDefinition> sysdef);

    virtual void analyze(unsigned int timestep);

  protected:
    const std::shared_ptr<Logger> m_logger;
  };
//! Export the Correlator class to python
void export_Correlator(pybind11::module& m);

// NO ENABLE_CUDA
#endif
