// inclusion guard
#ifndef _CORRELATOR_H_
#define _CORRELATOR_H_


#include <hoomd/Logger.h>
#include <hoomd/Analyzer.h>
#include <hoomd/SystemDefinition.h>


#ifndef NVCC
#include <hoomd/extern/pybind/include/pybind11/pybind11.h>
#endif

class Correlator : public Analyzer
  {
  public:
    //! Constructor
    Correlator(std::shared_ptr<Logger> logger, std::shared_ptr<SystemDefinition> sysdef);

    virtual void analyze(unsigned int timestep);

  protected:
    const std::shared_ptr<Logger> m_logger;
    const std::shared_ptr<SystemDefinition> m_sysdef;
  };
//! Export the Correlator class to python
void export_Correlator(pybind11::module& m);

// NO ENABLE_CUDA
#endif
