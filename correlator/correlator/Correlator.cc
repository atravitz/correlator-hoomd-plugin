#include "Correlator.h"

using namespace std;


Correlator::Correlator(std::shared_ptr<Logger> m_logger, std::shared_ptr<SystemDefinition> sysdef)
//  : m_sysdef(sysdef)
  : Analyzer(sysdef)
  {
  }


void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Correlator");

    // access the logged data values
    assert(m_logger);
    Scalar quantity = m_logger -> getQuantity();
//    ArrayHandle<Scalar> logged_values( m_logger -> getQuantity(), access_location::host, access_mode::read);
    quantity = m_logger -> getQuantity();
    

  }

void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator> >(m, "Correlator", pybind11::base<Analyzer>())
        .def(pybind11::init<std::shared_ptr<SystemDefinition> >());
  }
