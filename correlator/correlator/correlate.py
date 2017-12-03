# Copyright (c) 2009-2017 The Regents of the University of Michigan
# This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.

# this simple python interface just actiavates the c++ ExampleUpdater from cppmodule
# Check out any of the python code in lib/hoomd-python-module/hoomd_script for more examples

# First, we need to import the C++ module. It has the same name as this module (example_plugin) but with an underscore
# in front

# Next, since we are extending an analyzer, we need to bring in the base class analyzer and some other parts from
# hoomd_script
import hoomd
from hoomd import _hoomd
from hoomd.correlator import _correlator
##
#
class correlate(hoomd.analyze._analyzer):
    ## Initialize the correlator
    # \param filename - the name of the file containing output
    # \param quantity -  the logger describing the values to be sent to the autocorrelator
    # \param period - frequency that data is sent to the autocorrelator
    #
    # \b Examples:
    # \code
    # hoomd.correlator.correlate.correlate(filename='correlate.log', quantities=['potential_energy'], period=1)
    # \endcode

    def __init__(self, filename, quantities, period):
        hoomd.util.print_status_line()

        # initialize base class
        hoomd.analyze._analyzer.__init__(self)
        #hoomd.analyze._correlate.__init__(self)

        # convert quantities to a HOOMD vector string
        quantity_list = _hoomd.std_vector_string();
        for item in quantities:
            quantity_list.append(str(item));

        # initialize the reflected c++ class
        self.cpp_analyzer = _correlator.Correlator(hoomd.context.current.system_definition, filename, quantity_list, period)
        self.cpp_analyzer.setLoggedQuantities
        self.setupAnalyzer(period)

        # add the correlator to the list of loggers
        hoomd.context.current.loggers.append(self)

    # \brief Re-registers all computes and updaters with the logger
    def update_quantities(self):
        #remove all registered quantities
        self.cpp_analyzer.removeAll();

        #re-register all computes and updaters
        hoomd.context.current.system.registerLogger(self.cpp_analyzer)

    def disable(self):
        hoomd.util.print_status_line()

        hoomd.util.quiet_status()
        hoomd.util.unquiet_status()

        hoomd.analyze._analyzer.disable(self)
        hoomd.context.current.loggers.remove(self)
        hoomd.context.msg.notice(1, "correlator is disabled \n");

    def enable(self):
        hoomd.util.print_status_line()

        hoomd.util.quiet_status()
        hoomd.util.unquiet_status()

        hoomd.analyze._analyzer.enable(self)
        hoomd.context.current.loggers.append(self)
        hoomd.context.msg.notice(1, "correlator is enabled \n")

    #MAKE A FUNCTION THAT DUMPS THE DATA TO A FILE # FIX THIS
        # base this on logPlainTXT

    #ALLOW REGISTERING CALLBACKS
