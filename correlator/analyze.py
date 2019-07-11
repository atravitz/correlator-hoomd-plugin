# Copyright (c) 2009-2017 The Regents of the University of Michigan
# This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.

# this simple python interface just actiavates the c++ ExampleUpdater from cppmodule
# Check out any of the python code in lib/hoomd-python-module/hoomd_script for more examples

# First, we need to import the C++ module. It has the same name as this module (example_plugin) but with an underscore
# in front

# Next, since we are extending an analyzer, we need to bring in the base class analyzer and some other parts from hoomd_script
import hoomd
from hoomd import _hoomd
from hoomd.correlator import _correlator

## TODO: Allow registering callbacks
## TODO: allow change in period

class autocorrelate(hoomd.analyze._analyzer):
    ## Initialize the correlator
    # \param filename - the name of the file containing output
    # \param quantity -  the logger describing the values to be sent to the autocorrelator
    # \param period - frequency that data is sent to the autocorrelator
    #
    # \b Examples:
    # \code
    # hoomd.correlator.correlate.correlate(filename='correlate.log', quantities=['potential_energy'], period=1)
    # \endcode

    def __init__(self,  quantities, filename="autocorr.log", period=1, eval_period=0):
        hoomd.util.print_status_line()

        # initialize base class
        hoomd.analyze._analyzer.__init__(self)

        # convert quantities to a HOOMD vector string
        quantity_list = _hoomd.std_vector_string();
        for item in quantities:
            quantity_list.append(str(item));

        # initialize the reflected c++ class
        self.cpp_analyzer = _correlator.Correlator(hoomd.context.current.system_definition,                                                   filename,
                                                   quantity_list,
                                                   period,
                                                   eval_period)

        self.cpp_analyzer.setLoggedQuantities(quantity_list)
        self.setupAnalyzer(period)

        # add the correlator to the list of loggers
        hoomd.context.current.loggers.append(self)

    # \brief Re-registers all computes and updaters with the logger
    def update_quantities(self):
        #remove all registered quantities
        self.cpp_analyzer.removeAll();

        #re-register all computes and updaters
        hoomd.context.current.system.registerLogger(self.cpp_analyzer)

    # \brief disables the analyzer
    def disable(self):
        hoomd.util.print_status_line()

        hoomd.analyze._analyzer.disable(self)
        hoomd.context.current.loggers.remove(self)
        hoomd.context.msg.notice(1, "correlator is disabled \n");

    # \brief Re-enables the analyzer
    def enable(self):
        hoomd.util.print_status_line()

        hoomd.analyze._analyzer.enable(self)
        hoomd.context.current.loggers.append(self)
        hoomd.context.msg.notice(1, "correlator is enabled \n")

    # \brief writes output to file
    def evaluate(self):
        hoomd.util.print_status_line()
        self.cpp_analyzer.evaluate(hoomd.get_step())
