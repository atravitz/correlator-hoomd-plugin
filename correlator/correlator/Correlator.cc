#include "Correlator.h"
#include "correlator_likh.h"
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef, std::string filename, std::vector<std::string> quantities, unsigned int timestep, unsigned int eval_period)
  : Logger(sysdef), m_sysdef(sysdef), m_fname(filename), m_quantities(quantities), m_eval(eval_period), m_corr(32,16,4), m_is_initialized(false)
  {
    assert(m_sysdef);
    assert(m_fname);
    assert(m_quantities);
    assert(m_corr);
    assert(m_eval);

    m_corr.initialize();


  }

Correlator::~Correlator()
{
  cout << "Destructing Logger" << endl;
}


void Correlator::analyze(unsigned int timestep)
  {

    setLoggedQuantities(m_quantities);
    double value = this->getQuantity(m_quantities[0], timestep, false);
    m_corr.add(value);
    cout << m_corr.p << endl;
    // ofstream m_file;
    // m_file.open(m_fname);
    // m_file << logged_quantity;

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

//void Correlator::evaluate(unsigned int timestep, unsigned int eval_period)

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
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "Correlator", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition>, string , vector<std::string>, int, int  >());
  }
