import serial
import plotly.graph_objects as go
import time
from collections import deque
import pandas as pd
import plotly.express as px
import plotly.io as pio

pio.renderers.default = 'vscode'
# pio.renderers.default = 'iframe_connected'
pio.templates.default = 'plotly'


serial_port = 'COM4'  # یا هر پورت سریال دیگری که دستگاه شما به آن متصل است
baud_rate = 9600

# تنظیمات نمودار
max_points = 100  # تعداد حداکثر نقاط نمایش داده شده در نمودار

# اتصال به پورت سریال
ser = serial.Serial(serial_port, baud_rate)
num_data_points = 10
x_values = list(range(num_data_points))
PIRValues=[]
IRValues=[]
LDRValues=[]
LUXValues=[]
TemperatureValues=[]
fig = go.Figure()
while True:
    DataByte = ser.readline()
    DataString = DataByte.decode('utf-8', errors='replace').strip()  
    DataList = DataString.split()
    # print("Serial=")
    PIR=float(DataList[0].split('=')[1])
    IR=float(DataList[1].split('=')[1])
    LDR=float(DataList[2].split('=')[1])
    LUX=float(DataList[3].split('=')[1])
    Temperature=float(DataList[4].split('=')[1])
    PIRValues.append(PIR)
    IRValues.append(IR)
    LDRValues.append(LDR)
    LUXValues.append(LUX)
    TemperatureValues.append(Temperature)
    fig.add_trace(go.Scatter(x=x_values, y=PIRValues, mode='lines', name='PIR'))
    fig.add_trace(go.Scatter(x=x_values, y=IRValues, mode='lines', name='IR'))
    fig.add_trace(go.Scatter(x=x_values, y=LDRValues, mode='lines', name='LDR'))
    fig.add_trace(go.Scatter(x=x_values, y=LUXValues, mode='lines', name='LUX'))
    fig.add_trace(go.Scatter(x=x_values, y=TemperatureValues, mode='lines', name='Temperature'))
    fig.update_layout(title='Sensor Data', xaxis_title='Data Point', yaxis_title='Value')
    fig.show()
    time.sleep(0.5)
