import hoomd;
from hoomd import *
from hoomd import md
from hoomd import correlator

hoomd.context.initialize();
import unittest;
import os;
import logging
import errno

logging.basicConfig(level=logging.DEBUG) # uncomment this for debug mode
logger = logging.getLogger(__name__)
DISABLE_REMOVE = logger.isEnabledFor(logging.DEBUG)
DATA_DIR = os.path.join(os.path.dirname(__file__), 'test-py')

QUANTITIES = ['pressure']
FILENAME = 'outfile'

def silent_remove(filename, disable=False):
    """
    Removes the target file name, catching and ignoring errors that indicate that the
    file does not exist.

    @param filename: The file to remove.
    @param disable: boolean to flag if want to disable removal
    """
    if not disable:
        try:
            os.remove(filename)
        except OSError as e:
            if e.errno != errno.ENOENT:
                raise

class TestMain(unittest.TestCase):
    def testOutput(self):
        try:
          silent_remove(FILENAME)
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
          corr.evaluate()
        finally:
            silent_remove(FILENAME, disable=DISABLE_REMOVE)


if __name__ == '__main__':
    unittest.main(argv=['test_correlate.py', '-v'])
