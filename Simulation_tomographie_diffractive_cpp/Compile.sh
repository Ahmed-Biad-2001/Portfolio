#!/bin/bash
g++ src/*.cpp `pkg-config --cflags --libs opencv4` -o bin/Simulation
