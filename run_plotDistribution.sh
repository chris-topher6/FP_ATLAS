#!/bin/bash

# Array mit den Dateinamen der .root-Dateien
files=(
    "data.el"         "data.mu" 
    "ttbar.el"        "ttbar.mu" 
    "zprime400.el"    "zprime400.mu"
    "zprime500.el"    "zprime500.mu"
    "zprime750.el"    "zprime750.mu"
    "zprime1000.el"   "zprime1000.mu"
    "zprime1250.el"   "zprime1250.mu" 
    "zprime1500.el"   "zprime1500.mu" 
    "zprime1750.el"   "zprime1750.mu" 
    "zprime2000.el"   "zprime2000.mu"
    "zprime2250.el"   "zprime2250.mu"  
    "zprime2500.el"   "zprime2500.mu" 
    "zprime3000.el"   "zprime3000.mu" 
    "wjets.el"        "wjets.mu" 
    "zjets.el"        "zjets.mu" 
    "diboson.el"      "diboson.mu"
    "singletop.el"    "singletop.mu"
    "ttbar.el"        "ttbar.mu"
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