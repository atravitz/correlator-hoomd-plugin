// inclusion guard
#ifndef _CORRELATOR_H_
#define _CORRELATOR_H_


#include <hoomd/Logger.h>
#include <hoomd/Analyzer.h>
#include <hoomd/SystemDefinition.h>
#include "hoomd/Filesystem.h"


#ifndef NVCC
#include <hoomd/extern/pybind/include/pybind11/pybind11.h>
#endif

class Correlator : public Logger
  {
  public:
    //! Constructor
    Correlator(std::shared_ptr<SystemDefinition> sysdef, std::string filename, std::vector<std::string> quantities, unsigned int timestep);

    virtual void analyze(unsigned int timestep); //std::string filename, std::vector<std::string> quantities,

  protected:
    // const std::shared_ptr<Logger> Logger;
    const std::shared_ptr<SystemDefinition> m_sysdef;
    const std::vector<std::string> m_quantity;
    const std::string m_fname;
    const std::vector<std::string> m_quantities;
  };
//! Export the Correlator class to python
void export_Correlator(pybind11::module& m);

// NO ENABLE_CUDA
#endif
