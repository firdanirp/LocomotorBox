import serial   # For Serial communication
import time     # Required for using delay functions
import datetime  # For date-time setting and timedelta calculations

port = 'COM4'
baud = 9600
timeout = 10
filename = 'test.csv'

startYear1_1 = '2018'
startMonth1_1 = '11'
startdate1_1 = '13'
hourOn1_1 = '18'
minOn1_1 = '34'
t_cycle1_1 = '12'
days1_1 = '001'
ratio_on1_1 = '04'
ratio_off1_1 = '20'
dark1_1 = '0'
light1_1 = '0'
hourOn1_2 = '07'
minOn1_2 = '00'
t_cycle1_2 = '12'
days1_2 = '001'
ratio_on1_2 = '04'
ratio_off1_2 = '20'
dark1_2 = '0'
light1_2 = '0'
hourOn1_3 = '07'
minOn1_3 = '00'
t_cycle1_3 = '12'
days1_3 = '001'
ratio_on1_3 = '04'
ratio_off1_3 = '20'
dark1_3 = '0'
light1_3 = '0'


i = 0

serial_obj = serial.Serial(port, baud, timeout=timeout)
while True:
    string2 = serial_obj.readline().decode('utf-8')
    if string2 != '':
        with open(filename, 'a') as w:
            w.write(string2)
        w.close()
    print(string2)
    if i == 0:
        print('Synching time...')
        t = datetime.datetime.now()
        #serial_obj.write(str.encode(t.strftime('%Y-%m-%d %H:%M:%S')))
        serial_obj.write(str.encode('2018-11-14 23:56:00'))
    if i == 1:
        serial_obj.write(str.encode(startYear1_1+startMonth1_1+startdate1_1 +
                                    hourOn1_1+minOn1_1+t_cycle1_1+days1_1+ratio_on1_1+ratio_off1_1+dark1_1+light1_1))
    if i == 2:
        serial_obj.write(str.encode(hourOn1_2+minOn1_2+t_cycle1_2+days1_2+ratio_on1_2+ratio_off1_2+dark1_2+light1_2 +
                                    hourOn1_3+minOn1_3+t_cycle1_3+days1_3+ratio_on1_3+ratio_off1_3+dark1_3+light1_3))
    i = i+1
