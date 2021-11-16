#!/bin/bash

GHISTORYBITS=9
LHISTORYBITS=10
PCINDEXBITS=10

outfile_name=$(date +"%Y-%m-%d_%T");
path_to_outfile="../results/tournament/${outfile_name}.txt"

echo "Testing the tournament predictor on all test files..." >> $path_to_outfile
echo "ghistoryBits = ${GHISTORYBITS}" >> $path_to_outfile
echo "lhistoryBits = ${LHISTORYBITS}" >> $path_to_outfile
echo "pcIndexBits = ${PCINDEXBITS}" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile

for filename in ../traces/*; do
    echo "Test File: ${filename}" >> $path_to_outfile
    bunzip2 -kc $filename | ./../src/predictor --tournament:$GHISTORYBITS:$LHISTORYBITS:$PCINDEXBITS >> $path_to_outfile
    echo "---------------------------" >> $path_to_outfile
done
