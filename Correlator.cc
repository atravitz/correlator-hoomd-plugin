#include "Correlator.h"


Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef)
  : Analyzer(sysdef)
  {
  }

/* Perform autocorrelation calculations
 \param timestep Current time step of the simulation
 \param
*/

void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Log"); //profiler


  }
