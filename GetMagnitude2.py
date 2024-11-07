import math

# read the mapxt file and store the data in a list of tuples
with open('SolenoidField_GridMap_1cm.map') as f:
    data = [tuple(map(float, line.split())) for line in f]

# function to calculate the magnetic field strength
def field_strength(mx, my, mz):
    return math.sqrt(mx**2 + my**2 + mz**2)

# function to print the field data with position
def print_field_data(field_data):
    print("Num   Px    Py    Pz    Mx    My    Mz    Strength")
    print("-" * 50)
    for num, px, py, pz, mx, my, mz, strength in field_data:
        print(f"{num:5.0f}{px:6.2f}{py:6.2f}{pz:6.2f}{mx:7.4f}{my:7.4f}{mz:7.4f}{strength:9.4f}")


# function to find the strongest and weakest field data in a specified area
def find_extremes(x_start, x_end, y_start, y_end, z_start, z_end, num_results=10, strongest=True):
    field_data = []
    for num, px, py, pz, mx, my, mz in data:
        if x_start <= px <= x_end and y_start <= py <= y_end and z_start <= pz <= z_end:
            field_data.append((num, px, py, pz, mx, my, mz, field_strength(mx, my, mz)))
    field_data.sort(key=lambda x: x[7], reverse=strongest)
    return field_data[:num_results]

# Define search region
px_start = -10.0
px_end = 10.0
py_start = -10.0
py_end = 10.0
pz_start = -60.0
pz_end = 100.0

# find the 10 strongest and weakest field data in the entire grid
strongest_data = find_extremes(px_start, px_end, py_start, py_end, pz_start, pz_end)
weakest_data = find_extremes(px_start, px_end, py_start, py_end, pz_start, pz_end, num_results=10, strongest=False)

# find the 10 strongest and weakest field data at x=0 and y=0 for a given z range
strongest_zero_data = find_extremes(0, 0, 0, 0, pz_start, pz_end)
weakest_zero_data = find_extremes(0, 0, 0, 0, pz_start, pz_end, num_results=10, strongest=False)

# print the results
print("Top 10 strongest field data in the entire grid:")
print_field_data(strongest_data)
print("\nTop 10 weakest field data in the entire grid:")
print_field_data(weakest_data)
print("\nTop 10 strongest field data at x=0, y=0:")
print_field_data(strongest_zero_data)
print("\nTop 10 weakest field data at x=0, y=0:")
print_field_data(weakest_zero_data)

