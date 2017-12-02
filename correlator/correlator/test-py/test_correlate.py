import hoomd;
from hoomd import *
from hoomd import correlator
hoomd.context.initialize();
import unittest;
import os;


class test_simple(unittest.TestCase):
    def test_constructor(self):
        sysdef = hoomd.init.create_lattice(unitcell=hoomd.lattice.sq(a=2.0),
                                           n=[1,2]);
        corr = hoomd.correlator.correlate.correlate(filename='correlate.log', quantities=['potential_energy'], period=1)
        corr.disable()
        corr.enable()
        corr.update_quantities() ## not sure of the purpose of this
if __name__ == '__main__':
    unittest.main(argv = ['test_correlate.py', '-v'])
