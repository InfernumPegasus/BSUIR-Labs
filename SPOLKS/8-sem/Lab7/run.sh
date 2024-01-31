#!/bin/bash

source /etc/profile.d/modules.sh
module load mpi/openmpi-x86_64

mpic++ -std=c++20 main.cpp -o ${2}.out
mpirun -n ${1} ./${2}.out
