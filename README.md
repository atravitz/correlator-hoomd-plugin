# Correlator - on-the-fly autocorrelation for HOOMD-blue

## About

The Correlator plugin for HOOMD-blue allows for on-the-fly autocorrelation of any HOOMD quantity using the lihktman multiple tau autocorrelation algorithm.
This subverts the need to frequently dump state points for post-processing, reducing memory requirements and improving overall performance (TODO: is this true?)

## Installation

(TODO: how does one acquire a copy of our glorious code?)

git clone the bitbucket and add a symlink to your hoomd build

## Documentation

Correlator is a HOOMD plugin, as such, users should have a basic understanding of creating and running HOOMD scripts. 

    class hoomd.correlator.correlate.autocorrelate(quantities, 
                filename="corr.log", period=1, eval_period=0)
Autocorrelates a number of quantities

Parameters:
* filename(str) - File to write autocorrelation values to
* quantities(list) - List of quantities to autocorrelate
* period(int) - Data is correlated every period time steps
* eval_period(int) - Autocorrelation data is written every eval_period time steps

TODO: add information about .evaluate, .enable, .disable, and .updatequantities

Quantities that can be autocorrelated are the same as those that can be [logged](http://hoomd-blue.readthedocs.io/en/stable/module-hoomd-analyze.html)

A quick example:

```python
import hoomd
from hoomd import md
from hoomd import correlator
hoomd.context.initialize()

hoomd.init.create_lattice(unitcell=hoomd.lattice.sq(a=2.0), n=[1, 2])
group_all = group.all()
all = group=all
md.integrate.mode_standard(dt=0.01)
md.integrate.langevin(group=all, kT=1, seed=234)

corr = hoomd.correlator.correlate.autocorrelate(filename="corr.log", 
                    quantities="pressure", period=1, eval_period=10)
run(100)
corr.evaluate()
```

## Acknowledgement

TODO: make this section (more) official

Created by Alyssa Travitz and Alexander Adams

