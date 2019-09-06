import hoomd
from hoomd import *
from hoomd import md
from hoomd import correlator

hoomd.context.initialize()
import unittest
import os
import logging
import errno
import subprocess
import numpy as np
from fcc import fcc

# logging.basicConfig(level=logging.DEBUG) # uncomment this for debug mode
logger = logging.getLogger(__name__)
# DISABLE_REMOVE = logger.isEnabledFor(logging.DEBUG)
# DATA_DIR = os.path.join(os.path.dirname(__file__), "test-py")

QUANTITIES = ["pressure_xy"]
FILENAME = "corr_otf_test.txt"
OUTFILES_VALUES = ["outputtest.txt", "inputtest.txt", "pressure_xy.log", "corr.log"]


def silent_remove(filenames, disable=False):
    """
    Removes the target file name, catching and ignoring errors that indicate that the
    file does not exist.

    @param filename: The file to remove.
    @param disable: boolean to flag if want to disable removal
    """
    if not disable:
        for filename in filenames:
            try:
                os.remove(filename)
            except OSError as e:
                if e.errno != errno.ENOENT:
                    raise


# unit tests for autocorrelator
class test_correlate(unittest.TestCase):
    def setUp(self):
        context.initialize()
        self.system = hoomd.init.create_lattice(
            unitcell=hoomd.lattice.sq(a=2.0), n=[2, 2]
        )

    def test_create(self):
        """tests creation of the correlator """
        corr = hoomd.correlator.analyze.autocorrelate(
            filename=FILENAME, quantities=QUANTITIES, eval_period=1
        )

    def test_enable_disable(self):
        """tests enabling and disabling the correlator"""
        corr = hoomd.correlator.analyze.autocorrelate(
            filename=FILENAME, quantities=QUANTITIES, eval_period=1
        )

        corr.disable()
        corr.enable()

    def test_create_file(self):
        """tests that the correct output file is created"""
        all = group.all()
        md.integrate.mode_standard(dt=0.01)
        md.integrate.langevin(group=all, kT=1, seed=0)
        corr = hoomd.correlator.analyze.autocorrelate(
            filename=FILENAME, quantities=QUANTITIES, eval_period=1
        )
        run(10)
        corr.evaluate()
        self.assertTrue(os.path.isfile(FILENAME))

    def test_values(self):
        all = group.all()
        md.integrate.mode_standard(dt=0.01)
        md.integrate.langevin(group=all, kT=1, seed=0)
        corr = hoomd.correlator.analyze.autocorrelate(
            filename=FILENAME, quantities=QUANTITIES, eval_period=1
        )

        logger = analyze.log(
            filename="pressure_xy.log",
            quantities=["pressure_xy"],
            period=1,
            overwrite=True,
        )

        run(100)
        corr.evaluate()
        pressure_data = np.loadtxt("pressure_xy.log", skiprows=1, usecols=[1])
        np.savetxt("pressure_data.txt", pressure_data)
        subprocess.call(["./Correlator_IO", "pressure_data.txt", "corr_post_proc.txt"])

        corr_post_proc = np.loadtxt("corr_post_proc.txt")

        corr_otf = np.loadtxt("corr_otf_test.txt", skiprows=2, delimiter=",")

        np.testing.assert_almost_equal(corr_post_proc, corr_otf, decimal=2)
        silent_remove(["corr_post_proc.txt", "pressure_data.txt", "corr_otf_test.txt", "pressure_xy.log"])


if __name__ == "__main__":
    unittest.main(argv=["test_correlate.py", "-v"])

