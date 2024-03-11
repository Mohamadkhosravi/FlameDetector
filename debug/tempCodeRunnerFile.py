
serial_port = 'COM4'  
baud_rate = 9600
max_points = 1000  

ser = serial.Serial(serial_port, baud_rate)
num_data_points = 10
x_values = list(range(num_data_points))
PIRValues = deque(maxlen=max_points)
IRValues = deque(maxlen=max_points)
LDRValues = deque(maxlen=max_points)
LUXValues = deque(maxlen=max_points)
TemperatureValues = deque(maxlen=max_points)

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
    with fig.batch_update():
        fig.data[0].y = list(PIRValues)
        fig.data[1].y = list(IRValues)
        fig.data[2].y = list(LDRValues)
        fig.data[3].y = list(LUXValues)
        fig.data[4].y = list(TemperatureValues)
def read_serial_data():
    while True:
        data_bytes = ser.readline()
        data_string = data_bytes.decode('utf-8', errors='replace').strip()  
        data_list = data_string.split()
        PIR = float(data_list[0].split('=')[1])
        IR = float(data_list[1].split('=')[1])
        LDR = float(data_list[2].split('=')[1])
        LUX = float(data_list[3].split('=')[1])
        Temperature = float(data_list[4].split('=')[1])
        if PIR<=0:
            PIR=1
        if IR<=0:
            IR=1
        if LDR<=0:
            LDR=1
        if LUX<=0:
            LUX=1
        
        update_plot((PIR, IR, LDR, LUX, Temperature))
        time.sleep(0.2)
out = widgets.Output()
serial_thread = threading.Thread(target=read_serial_data)
serial_thread.start()
display(out)
with out:
    display(fig)
