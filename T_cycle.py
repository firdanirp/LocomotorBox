import serial   # For Serial communication
import time     # Required for using delay functions
import datetime  # For date-time setting and timedelta calculations

port = 'COM4'
baud = 9600
timeout = 10
filename = 'test.txt'

startYear1_1 = '2018'
startMonth1_1 = '11'
startdate1_1 = '13'
days1 = '001'
days2 = '001'
days3 = '001'
t_cycle1 = '12'
t_cycle2 = '12'
t_cycle3 = '24'


hourOn1_1 = '18'
minOn1_1 = '34'
hourOn2_1 = '18'
minOn2_1 = '34'
hourOn3_1 = '18'
minOn3_1 = '34'
hourOn4_1 = '18'
minOn4_1 = '34'
hourOn5_1 = '18'
minOn5_1 = '34'
hourOn6_1 = '18'
minOn6_1 = '34'
hourOn7_1 = '18'
minOn7_1 = '34'
hourOn8_1 = '18'
minOn8_1 = '34'
hourOn9_1 = '18'
minOn9_1 = '34'
hourOn10_1 = '18'
minOn10_1 = '34'

ratio_on1_1 = '04'
ratio_off1_1 = '20'
ratio_on2_1 = '04'
ratio_off2_1 = '20'
ratio_on3_1 = '04'
ratio_off3_1 = '20'
ratio_on4_1 = '04'
ratio_off4_1 = '20'
ratio_on5_1 = '04'
ratio_off5_1 = '20'
ratio_on6_1 = '04'
ratio_off6_1 = '20'
ratio_on7_1 = '04'
ratio_off7_1 = '20'
ratio_on8_1 = '04'
ratio_off8_1 = '20'
ratio_on9_1 = '04'
ratio_off9_1 = '20'
ratio_on10_1 = '04'
ratio_off10_1 = '20'

hourOn1_2 = '07'
minOn1_2 = '00'
hourOn2_2 = '07'
minOn2_2 = '00'
hourOn3_2 = '07'
minOn3_2 = '00'
hourOn4_2 = '07'
minOn4_2 = '00'
hourOn5_2 = '07'
minOn5_2 = '00'
hourOn6_2 = '07'
minOn6_2 = '00'
hourOn7_2 = '07'
minOn7_2 = '00'
hourOn8_2 = '07'
minOn8_2 = '00'
hourOn9_2 = '07'
minOn9_2 = '00'
hourOn10_2 = '07'
minOn10_2 = '00'

ratio_on1_2 = '04'
ratio_off1_2 = '20'
ratio_on2_2 = '04'
ratio_off2_2 = '20'
ratio_on3_2 = '04'
ratio_off3_2 = '20'
ratio_on4_2 = '04'
ratio_off4_2 = '20'
ratio_on5_2 = '04'
ratio_off5_2 = '20'
ratio_on6_2 = '04'
ratio_off6_2 = '20'
ratio_on7_2 = '04'
ratio_off7_2 = '20'
ratio_on8_2 = '04'
ratio_off8_2 = '20'
ratio_on9_2 = '04'
ratio_off9_2 = '20'
ratio_on10_2 = '04'
ratio_off10_2 = '20'

hourOn1_3 = '07'
minOn1_3 = '00'
hourOn2_3 = '07'
minOn2_3 = '00'
hourOn3_3 = '07'
minOn3_3 = '00'
hourOn4_3 = '07'
minOn4_3 = '00'
hourOn5_3 = '07'
minOn5_3 = '00'
hourOn6_3 = '07'
minOn6_3 = '00'
hourOn7_3 = '07'
minOn7_3 = '00'
hourOn8_3 = '07'
minOn8_3 = '00'
hourOn9_3 = '07'
minOn9_3 = '00'
hourOn10_3 = '07'
minOn10_3 = '00'

ratio_on1_3 = '04'
ratio_off1_3 = '20'
ratio_on2_3 = '04'
ratio_off2_3 = '20'
ratio_on3_3 = '04'
ratio_off3_3 = '20'
ratio_on4_3 = '04'
ratio_off4_3 = '20'
ratio_on5_3 = '04'
ratio_off5_3 = '20'
ratio_on6_3 = '04'
ratio_off6_3 = '20'
ratio_on7_3 = '04'
ratio_off7_3 = '20'
ratio_on8_3 = '04'
ratio_off8_3 = '20'
ratio_on9_3 = '04'
ratio_off9_3 = '20'
ratio_on10_3 = '04'
ratio_off10_3 = '20'

dark1_1 = '0'
light1_1 = '0'
dark2_1 = '0'
light2_1 = '0'
dark3_1 = '0'
light3_1 = '0'
dark4_1 = '0'
light4_1 = '0'
dark5_1 = '0'
light5_1 = '0'
dark6_1 = '0'
light6_1 = '0'
dark7_1 = '0'
light7_1 = '0'
dark8_1 = '0'
light8_1 = '0'
dark9_1 = '0'
light9_1 = '0'
dark10_1 = '0'
light10_1 = '0'

dark1_2 = '0'
light1_2 = '0'
dark2_2 = '0'
light2_2 = '0'
dark3_2 = '0'
light3_2 = '0'
dark4_2 = '0'
light4_2 = '0'
dark5_2 = '0'
light5_2 = '0'
dark6_2 = '0'
light6_2 = '0'
dark7_2 = '0'
light7_2 = '0'
dark8_2 = '0'
light8_2 = '0'
dark9_2 = '0'
light9_2 = '0'
dark10_2 = '0'
light10_2 = '0'

dark1_3 = '0'
light1_3 = '0'
dark2_3 = '0'
light2_3 = '0'
dark3_3 = '0'
light3_3 = '0'
dark4_3 = '0'
light4_3 = '0'
dark5_3 = '0'
light5_3 = '0'
dark6_3 = '0'
light6_3 = '0'
dark7_3 = '0'
light7_3 = '0'
dark8_3 = '0'
light8_3 = '0'
dark9_3 = '0'
light9_3 = '0'
dark10_3 = '0'
light10_3 = '0'

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
        serial_obj.write(str.encode('2018-15-11 23:57:20'))
    if i == 1:
        serial_obj.write(str.encode(startYear1_1 + startMonth1_1 + startdate1_1 +
                                    days1 + days2 + days3 + t_cycle1 + t_cycle2 + t_cycle3))

    if i == 2:
        serial_obj.write(str.encode(dark1_1 + light1_1 + dark2_1 + light2_1 + dark3_1 + light3_1 + dark4_1 + light4_1 + dark5_1 + light5_1 + dark6_1 + light6_1 +
                                    dark7_1 + light7_1 + dark8_1 + light8_1 + dark9_1 + light9_1 + dark10_1 + light10_1 +
                                    dark1_2 + light1_2 + dark2_2 + light2_2 + dark3_2 + light3_2 + dark4_2 + light4_2 + dark5_2 + light5_2 + dark6_2 + light6_2 +
                                    dark7_2 + light7_2 + dark8_2 + light8_2 + dark9_2 + light9_2 + dark10_2 + light10_2 +
                                    dark1_3 + light1_3 + dark2_3 + light2_3 + dark3_3 + light3_3 + dark4_3 + light4_3 + dark5_3 + light5_3 + dark6_3 + light6_3 +
                                    dark7_3 + light7_3 + dark8_3 + light8_3 + dark9_3 + light9_3 + dark10_3 + light10_3))
    if i == 3:
        serial_obj.write(str.encode(hourOn1_1 + minOn1_1 + hourOn2_1 + minOn2_1 + hourOn3_1 + minOn3_1 + hourOn4_1 + minOn4_1 + hourOn5_1 + minOn5_1 +
                                    hourOn6_1 + minOn6_1 + hourOn7_1 + minOn7_1 + hourOn8_1 + minOn8_1 + hourOn9_1 + minOn9_1 + hourOn10_1 + minOn10_1 +
                                    ratio_on1_1 + ratio_off1_1 + ratio_on2_1 + ratio_off2_1 + ratio_on3_1 + ratio_off3_1 + ratio_on4_1 + ratio_off4_1 + ratio_on5_1 + ratio_off5_1))
    if i == 4:
        serial_obj.write(str.encode(ratio_on6_1+ratio_off6_1 + ratio_on7_1 + ratio_off7_1 + ratio_on8_1 + ratio_off8_1 + ratio_on9_1 + ratio_off9_1 + ratio_on10_1 + ratio_off10_1 +
                                    hourOn1_2 + minOn1_2 + hourOn2_2 + minOn2_2 + hourOn3_2 + minOn3_2 + hourOn4_2 + minOn4_2 + hourOn5_2 + minOn5_2 +
                                    hourOn6_2 + minOn6_2 + hourOn7_2 + minOn7_2 + hourOn8_2 + minOn8_2 + hourOn9_2 + minOn9_2 + hourOn10_2 + minOn10_2))
    if i == 5:
        serial_obj.write(str.encode(ratio_on1_2 + ratio_off1_2 + ratio_on2_2 + ratio_off2_2 + ratio_on3_2 + ratio_off3_2 + ratio_on4_2 + ratio_off4_2 + ratio_on5_2 + ratio_off5_2 +
                                    ratio_on6_2 + ratio_off6_2 + ratio_on7_2 + ratio_off7_2 + ratio_on8_2 + ratio_off8_2 + ratio_on9_2 + ratio_off9_2 + ratio_on10_2 + ratio_off10_2 +
                                    hourOn1_3 + minOn1_3 + hourOn2_3 + minOn2_3 + hourOn3_3 + minOn3_3 + hourOn4_3 + minOn4_3 + hourOn5_3 + minOn5_3))
    if i == 6:
        serial_obj.write(str.encode(hourOn6_3 + minOn6_3 + hourOn7_3 + minOn7_3 + hourOn8_3 + minOn8_3 + hourOn9_3 + minOn9_3 + hourOn10_3 + minOn10_3 +
                                    ratio_on1_3 + ratio_off1_3 + ratio_on2_3 + ratio_off2_3 + ratio_on3_3 + ratio_off3_3 + ratio_on4_3 + ratio_off4_3 + ratio_on5_3 + ratio_off5_3 +
                                    ratio_on6_3 + ratio_off6_3 + ratio_on7_3 + ratio_off7_3 + ratio_on8_3 + ratio_off8_3 + ratio_on9_3 + ratio_off9_3 + ratio_on10_3 + ratio_off10_3))

    i = i+1
