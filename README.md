# Correlator - on-the-fly autocorrelation for HOOMD-blue

## About

The Correlator plugin for HOOMD-blue allows for on-the-fly autocorrelation of any HOOMD quantity using the lihktman multiple tau autocorrelation algorithm.
This subverts the need to frequently dump state points for post-processing, reducing memory requirements and improving overall performance (TODO: is this true?)

## Installation

(TODO: how does one acquire a copy of our glorious code?)

## Documentation

Correlator is a HOOMD plugin, as such, users should have a basic understanding of creating and running HOOMD scripts. 

A quick example:

    hoomd.initialize()
    corr = hoomd.correlator.correlate.autocorrelate(filename="corr.log", quantities="pressure", period=1)
    run(100)
    corr.evaluate

## Acknowledgement

TODO: make this section (more) official

Created by Alyssa Travitz and Alexander Adams

