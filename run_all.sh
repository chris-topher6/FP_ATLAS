#!/bin/bash

# führt Makefile aus
make 

# Führt das Skript run_selection.sh aus
./run_selection.sh

# Führt das Skript run_plotDistribution.sh aus
./run_plotDistribution.sh

# Führt stackedPlots.exe aus
./run_stackedPlots.sh

# Führt das Skript run_chiSquare.sh aus
./run_chiSquare.sh