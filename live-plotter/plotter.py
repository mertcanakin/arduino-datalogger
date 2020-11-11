import serial
import numpy
import matplotlib.pyplot as plt
from drawnow import *

ardu_data = serial.Serial('COM3', 38400)        # Initialize COM and Baud rate

## Create arrays for plotting data
gx_ar = [] 
gy_ar = []
gz_ar = []

ax_ar = []
ay_ar = []
az_ar = []

data_count = 0      # Counter for x axis
plt.ion()           # Get ready to plot live data   

def graph():        # Create a function to make plot

    plt.subplot(211)                                # Subplot for gyro values
    plt.grid(True)                                  # Add grid
    plt.ylabel('Gyro Values')                       # Rename y axis label
    plt.ylim(-20000, 20000)                         # Limit y axis 
    plt.plot(gx_ar, 'r', label = " X axis ")        # Plotting gyro values for each axis
    plt.plot(gy_ar, 'k', label = " Y axis ") 
    plt.plot(gz_ar, 'b', label = " Z axis ") 
    plt.legend(loc = 'upper left')                  # Location of plot legend

    plt.subplot(212)                                # Subplot fot accelerometer values
    plt.grid(True)
    plt.ylabel('Accelerometer Values')
    plt.ylim(-20000, 20000)
    plt.plot(ax_ar, 'r', label = " X axis ")
    plt.plot(ay_ar, 'k', label = " Y axis ") 
    plt.plot(az_ar, 'b', label = " Z axis ") 
    plt.legend(loc = 'upper left')

   

while True:                                     # Loop forever
    while (ardu_data.inWaiting() == 0):         # Wait until there is data
        pass
    ardu_string = ardu_data.readline()          # Read the coming line
    ardu_decoded = ardu_string.decode('utf-8')  # Decode data to make it readable
    data_array = ardu_decoded.split(',')        # Split incoming data to group them
    
    ##  Convert values to float
    ax = float(data_array[0]) 
    ay = float(data_array[1])
    az = float(data_array[2])

    gx = float(data_array[3]) 
    gy = float(data_array[4])
    gz = float(data_array[5])

    ## Append the values to their array
    gx_ar.append(gx)
    gy_ar.append(gy)
    gz_ar.append(gz)
    ax_ar.append(ax)
    ay_ar.append(ay)
    az_ar.append(az)

    drawnow(graph)                  # Draw the graph

    data_count = data_count + 1     # Add one to x axis counter

    if(data_count > 25):            # If counter > 25 delete first element of the array
        gx_ar.pop(0)
        gy_ar.pop(0)
        gz_ar.pop(0)
        ax_ar.pop(0)
        ay_ar.pop(0)
        az_ar.pop(0)

