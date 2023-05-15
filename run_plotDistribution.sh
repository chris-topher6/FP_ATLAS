#!/bin/bash

# Array mit den Dateinamen der .root-Dateien
files=("data.el.root" "data.mu.root" "ttbar.el.root" "ttbar.mu.root" "zprime1000.el.root" "zprime1000.mu.root")

# Schleife über die Dateien
for file in "${files[@]}"
do
    # Ausführung von plotDistribution für jede Datei
    ./plotDistribution.exe ~/E4/Final/samples/"$file"
done
