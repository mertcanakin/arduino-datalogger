import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

ardu_data = serial.Serial('COM3', 115200)        # Initialize COM and Baud rate

x_len = 600                 # Number of points to display
y_range1 = [0, 100]         # Range of possible Y values to display
y_range2 = [0, 100]         # Range of possible Y values to display
y_range3 = [-1,1]

fig = plt.figure()
ax1 = fig.add_subplot(2, 1, 1)
xs = list(range(0, 600))

ys1 = [0] * x_len
ys2 = [0] * x_len
ys3 = [0] * x_len

ax2 = ax1.twinx()

ax1.set_ylim(y_range1)
ax2.set_ylim(y_range2)
line1, = ax1.plot(xs, ys1,'b')
line2, = ax2.plot(xs, ys2,'r')
ax1.set_ylabel('Ultrasonic [cm]', color='b')
ax2.set_ylabel('Laser [cm]', color='r')
plt.yticks(np.arange(0, max(y_range1), 10))

plt.grid()
plt.title('Ultrasonic and Laser Sensor')
plt.xlabel('Samples')

ax3 = fig.add_subplot(2,1,2)
ax3.set_ylim(y_range3)
line3, = ax3.plot(xs, ys3,'k')
ax3.set_ylabel('Error [cm]', color='k')
plt.grid()
plt.title('Ultrasonic Sensor Error')
plt.xlabel('Samples')

def animate(i, ys1, ys2, ys3):
                                 
    while (ardu_data.inWaiting() == 0):             # Wait until there is data
        pass
    ardu_string = ardu_data.readline()              # Read the coming line
    
    ardu_decoded = ardu_string.decode('utf-8')      # Decode data to make it readable
    data_array = ardu_decoded.split(',')            # Split incoming data to group them
    
    ##  Convert values to float
    distance = data_array[0]
    error = data_array[1]
    laser = data_array[2]

    # Add y to list
    ys1.append(distance)
    ys2.append(laser)
    ys3.append(error)
    # Limit y list to set number of items
    ys1 = ys1[-x_len:]
    ys2 = ys2[-x_len:]
    ys3 = ys3[-x_len:]
    # Update line with new Y values
    line1.set_ydata(ys1)
    line2.set_ydata(ys2)
    line3.set_ydata(ys3)

    print(error + " ** " + distance + " ** " + laser)

    return line1, line2, line3,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,animate,fargs=(ys1,ys2,ys3),interval=50,blit=True)
plt.show()
        