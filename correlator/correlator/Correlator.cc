#include "Correlator.h"
#include "Logger.h"

using namespace std;


Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef)
  : Analyzer(sysdef)
  {
  }


void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Correlator"); //profiler

    // access the logged data values
    assert(m_logger);
    ArrayHandle<Scalar4> h_val(m_logger->getQuantity(), access_location::host, access_mode::read)

  }

void export_Correlator(pybind11:module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator> >(m, "Correlator", pybind11::base<Analyzer>())
        .def(pybind11::init<std::shared_ptr<SystemDefinition> >())
        ;
  }
