#include "Correlator.h"
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef, std::string filename, std::vector<std::string> quantities, unsigned int timestep)
  : Logger(sysdef), m_sysdef(sysdef), m_fname(filename), m_quantities(quantities)
  {
    assert(m_sysdef);
    assert(m_fname);
    assert(m_quantities);

    m_exec_conf->msg->notice(5) << "Constructing Correlator: "  << endl;
  }


void Correlator::analyze(unsigned int timestep)
  {
    setLoggedQuantities(m_quantities);
    Scalar logged_quantity = this->getQuantity(m_quantities[0], timestep, false);
    ofstream m_file;
    m_file.open(m_fname);
    m_file << logged_quantity;
    
  }


void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "Correlator", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition>, string , vector<std::string>, int  >());
  }
