import math

# Define a function to calculate the magnetic field strength
def get_magnitude(mx, my, mz):
    return math.sqrt(mx**2 + my**2 + mz**2)

# Read the file and extract the data
GridFileName = 'SolenoidField_GridMap_1cm.map'
data = []
with open(GridFileName, 'r') as f:
    for line in f:
        parts = line.split()
        num, px, py, pz, mx, my, mz = int(parts[0]), float(parts[1]), float(parts[2]), float(parts[3]), float(parts[4]), float(parts[5]), float(parts[6])
        data.append((num, px, py, pz, get_magnitude(mx, my, mz)))

# Sort the data by magnetic field strength
sorted_data = sorted(data, key=lambda x: x[4], reverse=True)

# Output the 10 largest magnetic field strengths with position information
print("Top 10 largest magnetic field strengths:")
for i in range(10):
    print("Position: ({}, {}, {}) Strength: {}".format(sorted_data[i][1], sorted_data[i][2], sorted_data[i][3], sorted_data[i][4]))

# Output the 10 smallest to tenth magnetic field strengths with position information
print("\nTop 10 smallest to tenth magnetic field strengths:")
for i in range(-10, 0):
    print("Position: ({}, {}, {}) Strength: {}".format(sorted_data[i][1], sorted_data[i][2], sorted_data[i][3], sorted_data[i][4]))

# Filter the data to only include points where x and y are zero
filtered_data = list(filter(lambda x: x[1] == 0 and x[2] == 0, data))

# Sort the filtered data by magnetic field strength
sorted_filtered_data = sorted(filtered_data, key=lambda x: x[4], reverse=True)

# Output the 10 largest magnetic field strengths at (0, 0) with position information
print("\nTop 10 largest magnetic field strengths at (0, 0):")
for i in range(10):
    print("Position: ({}, {}, {}) Strength: {}".format(sorted_filtered_data[i][1], sorted_filtered_data[i][2], sorted_filtered_data[i][3], sorted_filtered_data[i][4]))

# Output the 10 smallest magnetic field strengths at (0, 0) with position information
print("\nTop 10 smallest magnetic field strengths at (0, 0):")
for i in range(10):
    print("Position: ({}, {}, {}) Strength: {}".format(sorted_filtered_data[-i-1][1], sorted_filtered_data[-i-1][2], sorted_filtered_data[-i-1][3], sorted_filtered_data[-i-1][4]))

