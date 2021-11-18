#!/bin/bash

outfile_name=$(date +"%Y-%m-%d_%T");
path_to_outfile="../results/custom/${outfile_name}.txt"

echo "Testing the custom predictor on all test files..." >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile
echo "---------------------------" >> $path_to_outfile

for filename in ../traces/*; do
    echo "Test File: ${filename}" >> $path_to_outfile
    bunzip2 -kc $filename | ./../src/predictor --custom >> $path_to_outfile
    echo "---------------------------" >> $path_to_outfile
done
