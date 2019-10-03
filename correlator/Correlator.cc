#include "Correlator.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "correlator_likh.h"
using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef,
                       std::string filename,
                       std::vector<std::string> quantities, unsigned int period,
                       unsigned int eval_period)
    : Logger(sysdef),
      m_sysdef(sysdef),
      m_fname(filename),
      m_quantities(quantities),
      m_eval(eval_period),
      m_corr(32, 16, 2),
      m_is_initialized(false) {
  assert(m_sysdef);
  assert(m_fname);
  assert(m_quantities);
  assert(m_corr);

  m_corr.initialize();
}

Correlator::~Correlator() { cout << "Destructing Logger" << endl; }

void Correlator::analyze(unsigned int timestep) {
  setLoggedQuantities(m_quantities);
  double value = this->getQuantity(m_quantities[0], timestep, false);
  m_corr.add(value);
  // ofstream m_file;
  // m_file.open(m_fname);
  // m_file << logged_quantity;

  if (m_eval == 0) return;

  // brute force log every other timestep
  // if ((timestep) % (m_eval) == 0) {
  //   if (!m_is_initialized) {
  //     m_exec_conf->msg->notice(3)
  //         << "correlate.log: Creating new log in file \"" << m_fname << "\""
  //         << endl;
  //     m_file.open(m_fname.c_str(), ios_base::out);
  //   }

  //   m_is_initialized = true;
  //   m_corr.evaluate();
  //   m_file << "timestep: " << timestep;
  //   m_file << ("\n");
  //   for (unsigned int i = 0; i < m_corr.npcorr; ++i)
  //     m_file << m_corr.t[i] << " " << m_corr.f[i] << endl;
  //   m_file << ("\n");
  //   m_file.flush();
  // }
}

void Correlator::evaluate(unsigned int timestep) {
  if (!m_is_initialized) {
    m_exec_conf->msg->notice(3) << "correlate.log: Creating new log in file \""
                                << m_fname << "\"" << endl;
    m_file.open(m_fname.c_str(), ios_base::out);
  }

  m_is_initialized = true;
  m_corr.evaluate();

  // record values
  m_file << "correlator evaluated at timestep " << timestep << endl;
  m_file << "timestep,corr" << endl;
  for (unsigned int i = 0; i < m_corr.npcorr; ++i)
    m_file << m_corr.t[i] << "," << m_corr.f[i] << endl;
  m_file << ("\n");
  m_file.flush();
}

void export_Correlator(pybind11::module& m) {
  pybind11::class_<Correlator, std::shared_ptr<Correlator>>(
      m, "Correlator", pybind11::base<Logger>())
      .def(pybind11::init<shared_ptr<SystemDefinition>, string,
                          vector<std::string>, int, int>())
      .def("evaluate", &Correlator::evaluate);
}
