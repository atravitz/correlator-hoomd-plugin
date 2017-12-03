#include "Correlator.h"
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;

Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef, std::string filename, std::vector<std::string> quantities, unsigned int timestep)
  : Logger(sysdef), m_sysdef(sysdef), m_fname(filename)
  {
    assert(m_sysdef);



    m_exec_conf->msg->notice(5) << "Constructing Correlator: "  << endl;
  }


// void Correlator::correlate(std::string filename, std::vector<std::string> quantities, unsigned int timestep)
//   {
//     if (m_prof) m_prof->push("Correlator");
//     // if (filename == string(""))
//     //   m_file_output=false;
//     ofstream m_file;
//     m_file.open("filename.txt");
//     m_file << "beep boop. \n";
//     m_file.close();
//     cout << "Hi Alyssa" << endl;
//
//     // access the logged data values
//     // m_quantity(quantities)
//     // assert(m_quantity);
//     // Logger:setLoggedQuantities(m_quantity);
//
//     Scalar logged_quantity = this->getQuantity(quantities[0], timestep, false);
//
//
//     if (m_prof) m_prof->pop();
//
//     // m_file.open(m_filename.c_str(), ios_base::in | ios_base::out | ios_base::ate);
//   }

void Correlator::analyze(unsigned int timestep)
  {
    //
    // cout<<"screen output testing" << endl;
    ofstream m_file;
    m_file.open(m_fname);
    m_file << "testing output. \n";
    m_file.close();
    // Scalar logged_quantity = this->getQuantity(quantities[0], timestep, false);
  }


void export_Correlator(pybind11::module& m)
  {
    pybind11::class_<Correlator, std::shared_ptr<Correlator>>(m, "Correlator", pybind11::base<Logger>())
      .def(pybind11::init< shared_ptr<SystemDefinition>, string , vector<std::string>, int  >());
  }
