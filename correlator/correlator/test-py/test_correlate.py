import hoomd;
from hoomd import *
from hoomd import md
from hoomd import correlator

hoomd.context.initialize();
import unittest;
import os;

QUANTITIES = ['pressure']
FILENAME = "outfile"


class test_simple(unittest.TestCase):
    def test_constructor(self):
        sysdef = hoomd.init.create_lattice(unitcell=hoomd.lattice.sq(a=2.0), n=[1, 2]);
        group_all = group.all()
        all = group_all
        md.integrate.mode_standard(dt=0.01)
        md.integrate.langevin(group=all, kT=1, seed=234)
        # logger = analyze.log(filename='mylog.log', period=1, quantities=['volume'])

        # corr.disable()
        # corr.enable()
        # corr.update_quantities() ## not sure of the purpose of this

        corr = hoomd.correlator.correlate.autocorrelate(filename=FILENAME, quantities=QUANTITIES)
        run(100)


if __name__ == '__main__':
    unittest.main(argv=['test_correlate.py', '-v'])
