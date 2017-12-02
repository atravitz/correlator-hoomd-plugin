#include "Correlator.h"
#include <iomanip>
#include <stdexcept>
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
    // m_quantity(quantities)
    // assert(m_quantity);
    // Logger:setLoggedQuantities(m_quantity);

    Scalar logged_quantity = this->getQuantity(quantities[0], timestep, false);
    m_exec_conf->msg->notice(1) << "Constructing Correlator "  << endl;

    if (m_prof) m_prof->pop();

    // m_file.open(m_filename.c_str(), ios_base::in | ios_base::out | ios_base::ate);
  }



void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "CorrelateAnalyzer", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition> >());
  }
