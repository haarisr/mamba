#!/bin/bash

mkdir -p build
cmake -G Ninja -B build
cmake --build build
