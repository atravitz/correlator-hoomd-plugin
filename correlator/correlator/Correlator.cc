#include "Correlator.h"

using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef)
  : Logger(sysdef), m_sysdef(sysdef)
  {
    assert(m_sysdef);

  }


void Correlator::correlate(std::string filename, std::vector<std::string> quantities, unsigned int timestep)
  {
    if (m_prof) m_prof->push("Correlator");

    // access the logged data values
    //: m_quantity(quantities)
    //assert(m_quantity);
    //Logger:setLoggedQuantities(m_quantity);

    Scalar logged_quantity = this->getQuantity(quantities[0], timestep, false);

    if (m_prof) m_prof->pop();
  }



void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "CorrelateAnalyzer", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition> >());
  }
