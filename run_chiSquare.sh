#!/bin/bash

# Array mit den Massen des Z'
mass=(
    "400" "500" "750" "1000" "1250" "1500" "1750" "2000" "2250" "2500" "3000"
)

# Schleife über die Dateien
for i in "${!mass[@]}"
do
    ./chiSquare.exe plots/data.el_selected_plots.root stackedPlots/analysis_"${mass[i]}"_el.root
    ./chiSquare.exe plots/data.mu_selected_plots.root stackedPlots/analysis_"${mass[i]}"_mu.root
done

