#include "Correlator.h"
#include "correlator_likh.h"
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;

//TODO: make Correlator an analyzer
//TODO: give Correlator an m_logger object
Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef,
                       std::shared_ptr<Logger> logger,
                       std::string filename,
                       std::vector<std::string> quantities,
                       unsigned int period,
                       unsigned int eval_period)
  : Analyzer(sysdef), m_sysdef(sysdef), m_logger(logger),
    m_fname(filename), m_quantities(quantities), m_eval(eval_period),
    m_corr(32,16,2), m_is_initialized(false)
  {
    assert(m_sysdef);
    assert(m_fname);
    assert(m_quantities);
    assert(m_corr);

    m_corr.initialize();
  }


void Correlator::analyze(unsigned int timestep)
  {
    m_logger->setLoggedQuantities(m_quantities);
    double value = m_logger->getQuantity(m_quantities[0], timestep, false);
    m_corr.add(value);
    // ofstream m_file;
    // m_file.open(m_fname);
    // m_file << logged_quantity;

    if (m_eval==0)
        return;

    // brute force log every other timestep
    if ((timestep)%(m_eval) == 0)
        {
        if (! m_is_initialized)
            {
            m_exec_conf->msg->notice(3) << "correlate.log: Creating new log in file \"" << m_fname << "\"" << endl;
            m_file.open(m_fname.c_str(), ios_base::out);
            }

        m_is_initialized = true;
        m_corr.evaluate();
        m_file << "timestep: " << timestep;
        m_file << ("\n");
        for (unsigned int i=0;i<m_corr.npcorr;++i)
            m_file << m_corr.t[i] << " " << m_corr.f[i] << endl;
        m_file << ("\n");
        m_file.flush();
        }
  }

void Correlator::evaluate(unsigned int timestep)
    {
    if (! m_is_initialized)
      {
      m_exec_conf->msg->notice(3) << "correlate.log: Creating new log in file \"" << m_fname << "\"" << endl;
      m_file.open(m_fname.c_str(), ios_base::out);
      }

    m_is_initialized = true;
    m_corr.evaluate();
    m_file << "timestep: " << timestep << endl;
    for (unsigned int i=0;i<m_corr.npcorr;++i)
        m_file << m_corr.t[i] << " " << m_corr.f[i] << endl;
    m_file << ("\n");
    m_file.flush();
    }

// void Correlator::logging(unsigned int timestep)
//   {
//     if (! m_is_initialized)
//         {
//         m_exec_conf->msg->notice(3) << "correlate.log: Creating new log in file \"" << m_fname << "\"" << endl;
//         m_file.open(m_fname.c_str(), ios_base::out);
//         }
//
//     m_is_initialized = true;
//    m_corr->add(8);
//    m_file.open(m_fname);
//   m_file << value;


    // m_corr->add(value);
    //
    // ofstream m_file;
    // if (!m_file.is_open())
    //   m_file.open(m_fname);
    //
    // if (timestep == 1000)
    //   cout <<timestep << endl;
    //   m_corr->evaluate();
    //   cout<< m_corr->npcorr << endl;
    // for (unsigned int i=0; i<c.npcorr;++i)
    //
    //   m_file << c.t[i] << " " << c.f[i] << endl;
      // m_file.close();
//  }


void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "Correlator", pybind11::base<Analyzer>())
      .def(pybind11::init< shared_ptr<SystemDefinition>, shared_ptr<Logger>, string , vector<std::string>, int, int  >())
      .def("evaluate", &Correlator::evaluate)
      ;
  }
