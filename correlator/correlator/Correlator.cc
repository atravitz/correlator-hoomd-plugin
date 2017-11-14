#include "Correlator.h"
#include "Logger.h"

using namespace std;


Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef)
  : Analyzer(sysdef)
  {
  }



void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Log"); //profiler


  }
