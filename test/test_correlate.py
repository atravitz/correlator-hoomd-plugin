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
import pandas as pd
from fcc import fcc

# logging.basicConfig(level=logging.DEBUG) # uncomment this for debug mode
logger = logging.getLogger(__name__)
DISABLE_REMOVE = logger.isEnabledFor(logging.DEBUG)
DATA_DIR = os.path.join(os.path.dirname(__file__), "test-py")

QUANTITIES = ["pressure"]
FILENAME = "outfile"
OUTFILES_VALUES = ["outputtest.txt", "inputtest.txt", "pressure_xy.log", "corr.log"]


class TestMain(unittest.TestCase):
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

    def testOutput(self):
        try:
            TestMain.silent_remove(FILENAME)
            hoomd.init.create_lattice(unitcell=hoomd.lattice.sq(a=2.0), n=[1, 2])
            group_all = group.all()
            all = group_all
            md.integrate.mode_standard(dt=0.01)
            md.integrate.langevin(group=all, kT=1, seed=234)

            # Additional correlator functions
            # corr.disable()
            # corr.enable()

            corr = hoomd.correlator.analyze.autocorrelate(
                filename=FILENAME, quantities=QUANTITIES, eval_period=5
            )
            corr.disable()
            corr.enable()
            corr.update_quantities()
            run(100)
            corr.evaluate()
            self.assertTrue(os.path.isfile(FILENAME))
        finally:
            TestMain.silent_remove(FILENAME, disable=DISABLE_REMOVE)

    def testValues(self):
        try:
            fcc()
            path = os.getcwd()
            data = pd.read_table("pressure_xy.log", header=0, usecols=[1])
            data.pressure_xy.to_csv(str(path + "/inputtest.txt"), index=False)

            subprocess.call(
                [path + "/Correlator_IO", "inputtest.txt", "outputtest.txt"]
            )

            comparedata = pd.DataFrame()
            comparedata["postprocess"] = pd.read_table(
                "outputtest.txt",
                delim_whitespace=True,
                usecols=[1],
                header=None,
                names=["output"],
            )
            comparedata["onthefly"] = pd.read_table(
                "corr.log", skiprows=0, delim_whitespace=True, usecols=[1]
            )

            for n, val in comparedata.postprocess.iteritems():
                diff = val - comparedata.onthefly[n]
                self.assertFalse(diff >= 1e-6)

        finally:
            for OUT in OUTFILES_VALUES:
                TestMain.silent_remove(OUT, disable=DISABLE_REMOVE)


if __name__ == "__main__":
    unittest.main(argv=["test_correlate.py", "-v"])
