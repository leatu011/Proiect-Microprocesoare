from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import tkinter as tk
import numpy as np
import serial as sr
from tkinter import *
import time

#------global variables
data = np.array([])
cond = False
a=0
#-----plot data-----
def plot_data():
    global cond, data
    
    if (cond == True):
        
        a = s.readline()
        a.decode()
        
        if(len(data) < 100):
            data = np.append(data,float(a[0:4]))
        else:
            data[0:99] = data[1:100]
            data[99] = float(a[0:4])
		
        lines.set_xdata(np.arange(0,len(data)))
        lines.set_ydata(data)
        
        canvas.draw()
    
    root.after(1,plot_data)

def plot_start():
    global cond
    cond = True
    s.reset_input_buffer()

def plot_stop():
    s.write(b'r')
    global cond
    cond = False
    a = s.readline()
    a.decode()
    time.sleep(a[3]-48)
    i=a
    while i == a:
        a = s.readline()
        a.decode()
    label2=Label(text=a,font = ('calibri',12))
    label2.place(x=375,y=480)

    

#-----Main GUI code-----
root = tk.Tk()
root.title('DA5')
root.configure(background = 'light blue')
root.geometry("800x600")# set the window size

#------create Plot object on GUI----------
# add figure canvas
fig = Figure();
ax = fig.add_subplot(111)

#ax = plt.axes(xlim=(0,100),ylim=(0, 120)); #displaying only 100 samples
ax.set_title('Rotation');
ax.set_xlabel('Time')
ax.set_ylabel('Value')
ax.set_xlim(0,100)
ax.set_ylim(-0.5,11)
lines = ax.plot([],[])[0]

canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.get_tk_widget().place(x = 125,y=10, width = 550,height = 400)
canvas.draw()

label=Label(text="Counter value setting",font = ('calibri',12))
label.place(x=325,y=460)


root.update();
start = tk.Button(root, text = "Start", font = ('calibri',12),command = lambda: plot_start())
start.place(x = 200, y = 425 )

root.update();
stop = tk.Button(root, text = "Stop", font = ('calibri',12), command = lambda:plot_stop())
stop.place(x =600, y = 425)

#----start serial port----
s = sr.Serial('COM12',115200);
s.reset_input_buffer()



root.after(1,plot_data)
root.mainloop()