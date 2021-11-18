import os
import re

def get_measurements_from_file(path):
    measurements = []
    with open(path, 'r') as f:
        lines = f.readlines()
        for line in lines:
            if "Misprediction Rate:" in line:
                measurements.append(float(re.findall('\d*\.?\d+',line)[0]))
    return measurements

def compute_average(measurements):
    return sum(measurements)/len(measurements)

def append_to_file(path, average_measurement):
    with open(path, 'a') as f:
        f.write("Average Misprediction Rate: " + str(average_measurement))

def main(): 
    path = os.environ.get("OUTFILE")
    measurements = get_measurements_from_file(path)
    average_measurement = compute_average(measurements)
    append_to_file(path, average_measurement)

    predictor = path.split("/")[2]
    print("AVG MPR for " + str(predictor) + ": " + str(average_measurement))

if __name__ == "__main__":
    main()