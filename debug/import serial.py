import serial
import plotly.graph_objects as go
import time
import threading
import ipywidgets as widgets
from IPython.display import display
#1. delay among Data most be minimum 100ms 
#2. For RUN => [Right-click on the code and select "Run Current File in Interactive Window".]
serial_port = 'COM4'  
baud_rate = 9600

# =========================
# Change Scale
max_points = 100 
# =========================
ser = serial.Serial(serial_port, baud_rate)
num_data_points = 10
x_values = list(range(num_data_points))
PIRValues = []
IRValues = []
LDRValues = []
LUXValues = []
TemperatureValues = []

fig = go.FigureWidget()
fig.add_scatter(name='PIR')
fig.add_scatter(name='IR')
fig.add_scatter(name='LDR')
fig.add_scatter(name='LUX')
fig.add_scatter(name='Temperature')

def update_plot(data):
    PIR, IR, LDR, LUX, Temperature = data
    PIRValues.append(PIR)
    IRValues.append(IR)
    LDRValues.append(LDR)
    LUXValues.append(LUX)
    TemperatureValues.append(Temperature)
    
    # Update plot with the last `max_points` data points
    with fig.batch_update():
        fig.data[0].y = PIRValues[-max_points:]
        fig.data[1].y = IRValues[-max_points:]
        fig.data[2].y = LDRValues[-max_points:]
        fig.data[3].y = LUXValues[-max_points:]
        fig.data[4].y = TemperatureValues[-max_points:]

def read_serial_data():
    while True:
        try:
            data_bytes = ser.readline()
            data_string = data_bytes.decode('utf-8', errors='replace').strip()  
            data_list = data_string.split()
            PIR = float(data_list[0].split('=')[1])
            IR = float(data_list[1].split('=')[1])
            LDR = float(data_list[2].split('=')[1])
            LUX = float(data_list[3].split('=')[1])
            Temperature = float(data_list[4].split('=')[1])
            # if PIR <= 0:
            #     PIR = 1
            # if IR <= 0:
            #     IR = 1
            # if LDR <= 0:
            #     LDR = 1
            # if LUX <= 0:
            #     LUX = 1
            # if Temperature <= 0:
            #     Temperature = 1

            if PIR > 1024:
                PIR = 1024
            if IR  > 1024:
                IR = 1024
            if LDR  > 1024:
                LDR = 1024
            if LUX > 1024:
                LUX = 1024
            if Temperature > 1024:
                Temperature = 1024   

            # Add new data point
            update_plot((PIR, IR, LDR, LUX, Temperature))
            time.sleep(0.2)
            
        except Exception as e:
            print("An error occurred:", e)
            time.sleep(1)  


out = widgets.Output()
serial_thread = threading.Thread(target=read_serial_data)
serial_thread.start()

display(out)
with out:
    display(fig)
