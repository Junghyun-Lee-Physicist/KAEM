import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.utils import COMMASPACE, formatdate
from email import encoders

def send_email(myEmailAdd, to, subject, file_path):
    # Create the email structure
    msg = MIMEMultipart()
    msg['From'] = myEmailAdd
    msg['To'] = COMMASPACE.join(to)
    msg['Date'] = formatdate(localtime=True)
    msg['Subject'] = subject

    # Attach the file
    with open(file_path, 'rb') as f:
        part = MIMEBase('application', 'octet-stream', Name=file_path)
        part.set_payload((f).read())
        encoders.encode_base64(part)
        part.add_header('Content-Disposition', 'attachment', filename=file_path)
        msg.attach(part)

    # Send the email
    smtp = smtplib.SMTP('smtp.gmail.com', 587)
    smtp.starttls()
    smtp.login(myEmailAdd, 'adfasdfasdf')
    smtp.sendmail(myEmailAdd, to, msg.as_string())
    smtp.quit()




# Load the x, y, and z components of the magnetic field from the data file
GridNumber, x, y, z, Bx, By, Bz = np.loadtxt('SolenoidField_GridMap.map', unpack=True)

# Select the data points within the desired x-y-z range
mask = (x >= -10) & (x <= 10) & (y >= -10) & (y <= 10) & (z >= -10) & (z <= 10)
x, y, z, Bx, By, Bz = x[mask], y[mask], z[mask], Bx[mask], By[mask], Bz[mask]

# Set up the plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the vector field using quiver with a stride of 2
ax.quiver(x[::10], y[::10], z[::10], Bx[::10], By[::10], Bz[::10], length=1.0, normalize=True)

# Add labels and other plot details
ax.set_xlabel('x (cm)')
ax.set_ylabel('y (cm)')
ax.set_zlabel('z (cm)')
ax.set_title('Magnetic Field Vector Field')

# Save the plot
plt.savefig('plot.png', format='png')
# Example usage
send_email('tempwork12321@gmail.com',['dakdi93@knu.ac.kr'], 'PNG Image', 'plot.png')

