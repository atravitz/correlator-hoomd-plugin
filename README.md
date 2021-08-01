# On-the-fly autocorrelation for HOOMD-blue

## About

The Correlator plug-in for HOOMD-blue provides on-the-fly time autocorrelation of any HOOMD quantity using the [Likhtman multiple tau autocorrelation algorithm](https://aip.scitation.org/doi/10.1063/1.3491098).
This avoids the need to frequently dump state points for post-processing, reducing memory requirements and improving overall performance.

## Installation

Currently, this plug-in is only available for HOOMD v2.
To use this plug-in, clone the repository into a build of HOOMD-blue, symlink it
to the main HOOMD code and remake HOOMD.

    cd hoomd-blue
    git clone https://github.com/atravitz/correlator-hoomd-plugin
    cd hoomd
    ln -s ../correlator-hoomd-plugin/correlator .
    cd ../build
    cmake ../
    make -j4
    make install

## Documentation

Correlator is a HOOMD plug-in, as such, users should have a basic understanding of creating and running HOOMD scripts.

    class hoomd.correlator.correlate.autocorrelate(quantities,
                filename="autocorrelate.log", period=1, eval_period=0)

Parameters:
* `filename : str` - File to write autocorrelation values to
* `quantities : list` - List of quantities to autocorrelate
* `period : int` - Data is correlated every period time steps
* `eval_period : int` - Autocorrelation data is written every eval_period time steps

Additional autocorrelate functions are:

    autocorrelate.disable()

Disables the correlator

    autocorrelate.enable()

Enables the correlator

    autocorrelate.evalute()

Writes out autocorrelation values when called

Quantities that can be autocorrelated are the same as those that can be [logged](http://hoomd-blue.readthedocs.io/en/stable/module-hoomd-analyze.html)

A quick example:

```python
import hoomd
from hoomd import md
from hoomd import correlator
hoomd.context.initialize()

hoomd.init.create_lattice(unitcell=hoomd.lattice.sq(a=2.0), n=[1, 2])
md.integrate.mode_standard(dt=0.01)
md.integrate.langevin(group=hoomd.group.all(), kT=1, seed=234)

corr = hoomd.correlator.analyze.autocorrelate(filename="corr.log", quantities="pressure", period=1, eval_period=10)
hoomd.run(100)
corr.evaluate()
```

## Acknowledgement

Created by Alyssa Travitz and Alexander Adams
