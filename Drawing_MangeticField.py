import numpy as np
import matplotlib.pyplot as plt

# Load the x, y, and z components of the magnetic field from the data file
GridNumber, x, y, z, Bx, By, Bz = np.loadtxt('SolenoidField_GridMap.map', unpack=True)

# Select the data points within the desired x-y range and z range
mask = (x >= -10) & (x <= 10) & (y >= -10) & (y <= 10) & (z <= 10)
x, y, z, Bx, By, Bz = x[mask], y[mask], z[mask], Bx[mask], By[mask], Bz[mask]

# Calculate the magnitude of the magnetic field vector at each point
B = np.sqrt(Bx**2 + By**2 + Bz**2)

# Calculate the radial coordinate at each point
r = np.sqrt(x**2 + y**2)

# Set up the plot
fig, ax = plt.subplots()

# Plot the vector field using quiver
#ax.quiver(z, r, Bz, Bx, B, pivot='mid', units='inches')
ax.quiver(z, r, Bz, Bx, B)

# Add labels and other plot details
ax.set_xlabel('z (cm)')
ax.set_ylabel('r (cm)')
ax.set_title('Magnetic Field Vector Field')

# Save the plot
plt.savefig('plot.png', format='png')

