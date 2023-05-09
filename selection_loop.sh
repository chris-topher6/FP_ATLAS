#!/bin/bash
for filename in /net/e4-nfs-home.e4.physik.tu-dortmund.de/home/photon/E4/Final/samples/*; do
    if [ -f "$filename" ]; then
        ./runSelection.exe "$filename"
    fi
done
