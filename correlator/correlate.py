# Copyright (c) 2009-2017 The Regents of the University of Michigan
# This file is part of the HOOMD-blue project, released under the BSD 3-Clause License.

# this simple python interface just actiavates the c++ ExampleUpdater from cppmodule
# Check out any of the python code in lib/hoomd-python-module/hoomd_script for moreexamples

# First, we need to import the C++ module. It has the same name as this module (example_plugin) but with an underscore
# in front
from hoomd.correlate import _correlate

# Next, since we are extending an analyzer, we need to bring in the base class analyzer and some other parts from
# hoomd_script
import hoomd

## Zeroes all particle velocities
#
# Every \a period time steps, particle velocities are modified so that they are all zero
#
class correlate(hoomd.analyze._analyzer):
    ## Initialize the velocity zeroer
    #
    # \param period Velocities will be zeroed every \a period time steps
    #
    # \b Examples:
    # \code
    # example_plugin.update.example()
    # zeroer = example_plugin.update.example(period=10)
    # \endcode
    #
    # \a period can be a function: see \ref variable_period_docs for details
    def __init__(self, period=1):
        hoomd.util.print_status_line();

        # initialize base class
        hoomd.analyze._analyzer.__init__(self);

        # initialize the reflected c++ class
        self.cpp_analyzer = _correlator.CorrelateAnalyzer(hoomd.context.current.system_definition);

        self.setupAnalyzer(period);
