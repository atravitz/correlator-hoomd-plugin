#include "Correlator.h"

using namespace std;


Correlator::Correlator(std::shared_ptr<SystemDefinition> sysdef)
  : Analyzer(sysdef)
  {
  }


/* pulled from HOOMD Logger.cc:
  \param compute The Compute to register
  After the compute is registered, all of the compute's provided log quantities are available for
  logging.
*/
void Correlator::registerCompute(std::shared_ptr<Compute> compute)
    {
    vector< string > provided_quantities = compute->getProvidedLogQuantities();

    // loop over all log quantities
    for (unsigned int i = 0; i < provided_quantities.size(); i++)
        {
        // first check if this quantity is already set, printing a warning if so
        if (   m_compute_quantities.count(provided_quantities[i])
            || m_updater_quantities.count(provided_quantities[i])
            || m_callback_quantities.count(provided_quantities[i])
            )
            m_exec_conf->msg->warning() << "analyze.log: The log quantity " << provided_quantities[i] <<
                 " has been registered more than once. Only the most recent registration takes effect" << endl;
        m_compute_quantities[provided_quantities[i]] = compute;
        m_exec_conf->msg->notice(6) << "analyze.log: Registering log quantity " << provided_quantities[i] << endl;
        }
    }

/* pulled from HOOMD Logger.cc:
/*! \param quantities A list of quantities to log
    When analyze() is called, each quantitiy in the list will, in order, be requested
    from the matching registered compute or updater.
*/

void Logger::setLoggedQuantities(const std::vector< std::string >& quantities)
    {
    m_logged_quantities = quantities;

    // prepare or adjust storage for caching the logger properties.
    m_cached_timestep = -1;
    m_cached_quantities.resize(quantities.size());
    }


/* Perform autocorrelation calculations
 \param timestep Current time step of the simulation
 \param
*/

void Correlator::analyze(unsigned int timestep)
  {
    if (m_prof) m_prof->push("Log"); //profiler


  }
