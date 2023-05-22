#!/bin/bash

# Array mit den Dateinamen der .root-Dateien
files=(
    "data.el"         "data.mu" 
    "ttbar.el"        "ttbar.mu" 
    "zprime400.el"    "zprime400.mu" 
    "zprime1000.el"   "zprime1000.mu" 
    "wjets.el"        "wjets.mu" 
    "zjets.el"        "zjets.mu" 
    "diboson.el"      "diboson.mu"
    )

# Schleife über die Dateien in ~/E4/Final/samples/
for file in "${files[@]}"
do
    ./plotDistribution.exe ~/E4/Final/samples/"$file".root
done

# Schleife über die Dateien in ~/Breitfeld_Knospe_26-04-23/output_runSelection
for file in "${files[@]}"
do
    ./plotDistribution.exe ~/Breitfeld_Knospe_26-04-23/output_runSelection/"$file"_selected.root
done