#!/bin/bash

echo calling setupCVMFSGENIE_v3_00_06.sh

source /cvmfs/larsoft.opensciencegrid.org/products/setup
setup genie v3_00_06d -q e17:prof

echo GENIE: $GENIE

export GENIEbase=/data/t2k/xlu/alltorm/GENIE
export myGENIE=${GENIEbase}/inuse
export GENIEspline=${myGENIE}/spline

export PATH=${GENIE}/bin:$PATH
export LD_LIBRARY_PATH=${GENIE}/lib:$LD_LIBRARY_PATH
