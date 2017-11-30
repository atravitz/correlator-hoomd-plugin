#include "Correlator.h"

using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef, std::string quantity, std::filename)
  : Analyzer(sysdef), m_logger(logger), m_sysdef(sysdef), m_quantity(quantity)
  {
    assert(m_logger);
    assert(m_sysdef);
    //set logged quantites
  }


void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Correlator");

    // access the logged data values
    assert(m_logger);


    Scalar logged_quantity = m_logger->getQuantity(m_quantity, timestep, false);
    # pop
  }



void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "CorrelateAnalyzer", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition> >());
  }
//match to Correlator:Correlator
