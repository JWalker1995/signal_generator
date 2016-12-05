#!/bin/sh

mpic++ main.cpp -std=c++11 -O3 -g -o signal_generator
mpirun -n 16 signal_generator

