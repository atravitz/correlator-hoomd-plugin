# Copyright (c) 2009-2017 The Regents of the University of Michigan
# This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.

# this simple python interface just actiavates the c++ ExampleUpdater from cppmodule
# Check out any of the python code in lib/hoomd-python-module/hoomd_script for more examples

# First, we need to import the C++ module. It has the same name as this module (example_plugin) but with an underscore
# in front

# Next, since we are extending an analyzer, we need to bring in the base class analyzer and some other parts from
# hoomd_script
import hoomd

##
#
class correlator(hoomd.analyze._analyzer):
    ## Initialize the correlator
    #
    # \param logger -  the logger describing the values to be sent to the autocorrelator
    #
    # \b Examples:
    # \code
    # hoomd.analyze.correlate(log)
    # \endcode

    def __init__(self, quantity, period):
        hoomd.util.print_status_line()

        # initialize base class
        hoomd.analyze._analyzer.__init__(self)

        # initialize the reflected c++ class
        self.cpp_analyzer = _correlator.CorrelateAnalyzer(logger.cpp_analyzer, hoomd.context.current.system_definition)
        self.setupAnalyzer(period)
        ## context.currentloggers.append()
##update quantites from analyze.py
