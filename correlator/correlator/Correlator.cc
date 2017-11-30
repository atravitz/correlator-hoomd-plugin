#include "Correlator.h"

using namespace std;

Correlator::Correlator(std::shared_ptr<Logger> logger, std::shared_ptr<SystemDefinition> sysdef)
  : Analyzer(sysdef), m_logger(logger), m_sysdef(sysdef)
  {
    assert(m_logger);
    assert(m_sysdef);
  }


void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Correlator");

    // access the logged data values
    assert(m_logger);
    std::string q;

    Scalar quantity = m_logger->getQuantity(q, timestep, false);
  }



void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "CorrelateAnalyzer", pybind11::base<Analyzer>())
      .def(pybind11::init< shared_ptr<Logger>, shared_ptr<SystemDefinition> >());
  }
