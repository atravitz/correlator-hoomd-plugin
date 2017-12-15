from hoomd import *
from hoomd import md
from hoomd import deprecated
from hoomd import correlator
import math as m
import numpy as np
import sys
import pandas as pd
import subprocess
from test_fcc import fcc

fcc()
path = os.getcwd()
data = pd.read_table("pressure_xy.log", header = 0, usecols=[1])
data.pressure_xy.to_csv(str(path+'/inputtest.txt'), index =False)

subprocess.call([path+'/Correlator_IO', 'inputtest.txt', 'outputtest.txt'])

comparedata = pd.DataFrame()
comparedata['postprocess'] = pd.read_table('outputtest.txt', delim_whitespace = True, usecols=[1], header = None, names = ['output'])
comparedata['onthefly'] = pd.read_table('corr.log',skiprows=0, delim_whitespace = True, usecols=[1])

for n, val in comparedata.postprocess.iteritems():
    diff = val - comparedata.onthefly[n]
    if diff >= 1e-6:
        print('TEST FAIL')
        sys.exit()

print('TEST PASS')
os.remove("outputtest.txt")
os.remove("inputtest.txt")
