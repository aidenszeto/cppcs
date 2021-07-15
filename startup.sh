#!/bin/bash

shopt -s extglob
g++ ./cppcs/!(cache).cpp -o run
./run
