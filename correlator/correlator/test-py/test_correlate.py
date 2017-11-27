import hoomd;
hoomd.context.initialize();
import hoomd.correlate;
import unittest;
import os;

class test_simple(unittest.TestCase):
    def test_constructor(self):
        sysdef = hoomd.init.create_lattice(unitcell=hoomd.lattice.sq(a=2.0),
                                           n=[1,2]);
        log = hoomd.analyze.log(filename=None, quantities='potential_energy', period=1)

        correlate = hoomd.correlate.analyze(log)



if __name__ == '__main__':
    unittest.main(argv = ['test_example.py', '-v'])
