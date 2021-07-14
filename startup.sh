#!/bin/bash

shopt -s extglob
g++ ./src/cppcs/!(cache).cpp -o run
./run
