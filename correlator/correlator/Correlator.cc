#include "Correlator.h"

using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef, std::string filename, std::vector<std::string> quantity, int period)
  : Logger(sysdef), m_sysdef(sysdef), m_quantity(quantity)
  {
    assert(m_sysdef);
    assert(m_quantity);
    Logger:setLoggedQuantities(m_quantity);

  }


void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Correlator");

    // access the logged data values
    assert(m_quantity);

    Scalar logged_quantity = this->getQuantity(m_quantity[0], timestep, false);

    if (m_prof) m_prof->pop();
  }



void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "CorrelateAnalyzer", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition>>, string<filename>, vector<std::string> <quantity>, string<period> >());
  }
