#!/bin/bash

GHISTORYBITS=13

outfile_name=$(date +"%Y-%m-%d_%T");
path_to_outfile="../results/gshare/${outfile_name}.txt"

echo "Testing the gshare predictor on all test files..." >> $path_to_outfile
echo "ghistoryBits = ${GHISTORYBITS}" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile

for filename in ../traces/*; do
    echo "Test File: ${filename}" >> $path_to_outfile
    bunzip2 -kc $filename | ./../src/predictor --gshare:$GHISTORYBITS >> $path_to_outfile
    echo "---------------------------" >> $path_to_outfile
done

export OUTFILE=$path_to_outfile
python ../tests/average.py
