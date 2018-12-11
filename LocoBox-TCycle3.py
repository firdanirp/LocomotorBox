import serial   # For Serial communication
import time     # Required for using delay functions
import datetime  # For date-time setting and timedelta calculations
import platform
import glob
import tkinter as tk
from tkinter import *  # import INIT set of tkinter library for GUI
from tkinter import ttk
from tkinter import messagebox
from tkinter.filedialog import askopenfilename
import json
try:
    from tkinter import filedialog
except ImportError:
    fileDialog = tk.filedialog
import threading  # To run Arduino loop and tkinter loop alongside
import serial.tools.list_ports  # For identifying Arduino port
import numpy as np
import pandas as pd


# Global variables
global starthour, startminute, startYear1_1, startMonth1_1, startdate1_1, days1, days2, days3, t_cycle1, t_cycle2, t_cycle3
global hourOn_1, minOn_1
global hourOn_2, minOn_2
global hourOn_3, minOn_3
global ratio_on_1, ratio_off_1
global ratio_on_2, ratio_off_2
global ratio_on_3, ratio_off_3
global dark_1, light_1
global dark_2, light_2
global dark_3, light_3
global setBox

# Preset values
setBox=0

# Version information


def about():
    return messagebox.showinfo('About',
                                '10-Box Schedule Setter\n' +
                                'LocoBox-TCycle.py\n\n' +
                                'Version 0.1.0\n' +
                                'Nov 19, 2018\n\n' +
                                'Jihwan Myung & Vuong Truong\n' +
                                'Laboratory of Braintime\n\n' +
                                'https://github.com/braintimelab/LocomotorBox')

# Define and create serial object function


def create_serial_obj(portPath, baud_rate, timeout):
    '''
    Given the port path, baud rate, creates
    and returns a pyserial object.
    '''
    return serial.Serial(portPath, baud_rate, timeout=timeout)

# Find open serial ports


def serial_ports():
    arduino_ports = [
        p.device
        for p in serial.tools.list_ports.comports()
        if 'Arduino' or 'Generic CDC' in p.description
    ]
    if sys.platform.startswith('win'):
        arduino_ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal '/dev/tty'
        arduino_ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        arduino_ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in arduino_ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

    # Classes


class StatusBar(Frame):  # scan open serial ports
    def __init__(self, master):
        Frame.__init__(self, master)
        self.label = Label(self, bd=1, relief=SUNKEN, anchor=W)
        self.label.pack(fill=X)

    def set(self, format, *args):
        self.label.config(text=format % args)
        self.label.update_idletasks()

    def clear(self):
        self.label.config(text='')
        self.label.update_idletasks()


# Initialize the windows size and name
window = Tk()
window.title('LocoBox (10-box)')
if sys.platform.startswith('win'):
    window.geometry('770x420')
elif sys.platform.startswith('darwin'):
    window.geometry('1000x440')
elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
    window.geometry('730x420')
else:
    window.geometry('1000x440')
status = StatusBar(window)

# Define functions


def destruct():  # Quit the program
    print('LocoBox ended.')
    window.quit()


def get_data(istate=0):  # Start recording
    status.pack(side='bottom', fill='x')
    status.set('Starting the recording...')
    for i in range(10):
        boxrec_text[i].set('Preparing for recording.')

    window.update_idletasks()
    i = istate
    counti = 0
    # init csv file and write the COM port name
    headers = port_entry.get()
    filename = filename_entry.get()
    with open(filename, 'w', encoding='utf-8') as w:
                w.write(headers+'\n')
    w.close()
    global serial_obj
    global dead
    # try:
    while True:
        string2 = serial_obj.readline().decode('utf-8')
        if string2 != '':
            with open(filename, 'a') as w:
                w.write(string2)
            w.close()
        print(string2)
        if i == 0:
            print('Synching time...')
            status.pack(side='bottom', fill='x')
            status.set('Synching time...')
            t = datetime.datetime.now()
            serial_obj.write(str.encode(t.strftime('%Y-%m-%d %H:%M:%S')))
            #serial_obj.write(str.encode(t.strftime('2018-12-08 23:58:30')))
            
        if i == 1:
            serial_obj.write(str.encode(starthour+startminute+startYear1_1 + startMonth1_1 + startdate1_1 +
                                        days1 + days2 + days3 + t_cycle1 + t_cycle2 + t_cycle3))

        if i == 2:
            serial_obj.write(str.encode(dark_1[0] + light_1[0] + dark_1[1] + light_1[1] + dark_1[2] + light_1[2] + dark_1[3] + light_1[3] + dark_1[4] + light_1[4] + dark_1[5] + light_1[5] +
                                        dark_1[6] + light_1[6] + dark_1[7] + light_1[7] + dark_1[8] + light_1[8] + dark_1[9] + light_1[9] +
                                        dark_2[0] + light_2[0] + dark_2[1] + light_2[1] + dark_2[2] + light_2[2] + dark_2[3] + light_2[3] + dark_2[4] + light_2[4] + dark_2[5] + light_2[5] +
                                        dark_2[6] + light_2[6] + dark_2[7] + light_2[7] + dark_2[8] + light_2[8] + dark_2[9] + light_2[9] +
                                        dark_3[0] + light_3[0] + dark_3[1] + light_3[1] + dark_3[2] + light_3[2] + dark_3[3] + light_3[3] + dark_3[4] + light_3[4] + dark_3[5] + light_3[5] +
                                        dark_3[6] + light_3[6] + dark_3[7] + light_3[7] + dark_3[8] + light_3[8] + dark_3[9] + light_3[9]))
        if i == 3:
            serial_obj.write(str.encode(hourOn_1[0] + minOn_1[0] + hourOn_1[1] + minOn_1[1] + hourOn_1[2] + minOn_1[2] + hourOn_1[3] + minOn_1[3] + hourOn_1[4] + minOn_1[4] +
                                        hourOn_1[5] + minOn_1[5] + hourOn_1[6] + minOn_1[6] + hourOn_1[7] + minOn_1[7] + hourOn_1[8] + minOn_1[8] + hourOn_1[9] + minOn_1[9] +
                                        ratio_on_1[0] + ratio_off_1[0] + ratio_on_1[1] + ratio_off_1[1] + ratio_on_1[2] + ratio_off_1[2] + ratio_on_1[3] + ratio_off_1[3] + ratio_on_1[4] + ratio_off_1[4]))
        if i == 4:
            serial_obj.write(str.encode(ratio_on_1[5]+ratio_off_1[5] + ratio_on_1[6] + ratio_off_1[6] + ratio_on_1[7] + ratio_off_1[7] + ratio_on_1[8] + ratio_off_1[8] + ratio_on_1[9] + ratio_off_1[9] +
                                        hourOn_2[0] + minOn_2[0] + hourOn_2[1] + minOn_2[1] + hourOn_2[2] + minOn_2[2] + hourOn_2[3] + minOn_2[3] + hourOn_2[4] + minOn_2[4] +
                                        hourOn_2[5] + minOn_2[5] + hourOn_2[6] + minOn_2[6] + hourOn_2[7] + minOn_2[7] + hourOn_2[8] + minOn_2[8] + hourOn_2[9] + minOn_2[9]))
        if i == 5:
            serial_obj.write(str.encode(ratio_on_2[0] + ratio_off_2[0] + ratio_on_2[1] + ratio_off_2[1] + ratio_on_2[2] + ratio_off_2[2] + ratio_on_2[3] + ratio_off_2[3] + ratio_on_2[4] + ratio_off_2[4] +
                                        ratio_on_2[5] + ratio_off_2[5] + ratio_on_2[6] + ratio_off_2[6] + ratio_on_2[7] + ratio_off_2[7] + ratio_on_2[8] + ratio_off_2[8] + ratio_on_2[9] + ratio_off_2[9] +
                                        hourOn_3[0] + minOn_3[0] + hourOn_3[1] + minOn_3[1] + hourOn_3[2] + minOn_3[2] + hourOn_3[3] + minOn_3[3] + hourOn_3[4] + minOn_3[4]))
        if i == 6:
            serial_obj.write(str.encode(hourOn_3[5] + minOn_3[5] + hourOn_3[6] + minOn_3[6] + hourOn_3[7] + minOn_3[7] + hourOn_3[8] + minOn_3[8] + hourOn_3[9] + minOn_3[9] +
                                        ratio_on_3[0] + ratio_off_3[0] + ratio_on_3[1] + ratio_off_3[1] + ratio_on_3[2] + ratio_off_3[2] + ratio_on_3[3] + ratio_off_3[3] + ratio_on_3[4] + ratio_off_3[4] +
                                        ratio_on_3[5] + ratio_off_3[5] + ratio_on_3[6] + ratio_off_3[6] + ratio_on_3[7] + ratio_off_3[7] + ratio_on_3[8] + ratio_off_3[8] + ratio_on_3[9] + ratio_off_3[9]))
            status.pack(side='bottom', fill='x')
            status.set('All schedules transferred. Recording began.')
            for i in range(10):
                boxrec_text[i].set('Recording on-going.')

            window.update_idletasks()
        i = i+1

    # except:
    #     print('Stopped recording and disconnected from the boxes.')
    #     status.pack(side='bottom', fill='x')
    #     status.set('Stopped recording and disconnected from the boxes.') 
    #     for i in range(10):
    #         boxrec_text[i].set('Recording stopped.')
    #     window.update_idletasks()


def connect():  # Start to connect and call get_data - Link to Start in Recording menu
    port = port_entry.get()
    baud = baud_entry.get()
    timeout = int(timeout_entry.get())
    global serial_obj
    global dead
    dead = False
    try:
        serial_obj = create_serial_obj(port, baud, timeout=timeout)
    except NameError:
        status.pack(side='bottom', fill='x')
        status.set('Missing baud rate and port number.')
        return
    t1 = threading.Thread(target=lambda: get_data(0))
    t1.daemon = True
    # inactivate Recording Start button
    btnRun['state'] = 'disabled'
    recordingmenu.entryconfig('Start new', state='disabled')
    # show_conf()
    window.update_idletasks()
    t1.start()


def disconnect():  # close the serial_obj thread
    status.pack(side='bottom', fill='x')
    status.set('Attempting to close serial port...')
    global dead
    global serial_obj
    dead = True
    serial_obj.close()
    print(threading.active_count())
    print(threading.enumerate())
    status.pack(side='bottom', fill='x')
    status.set('Stopped recording and disconnected from the boxes.')
    for i in range(10):
        boxrec_text[i].set('Recording stopped.')
    window.update_idletasks()


def getAllBoxSchedule():
    global starthour, startminute, startYear1_1, startMonth1_1, startdate1_1, days1, days2, days3, t_cycle1, t_cycle2, t_cycle3
    global hourOn_1, minOn_1
    global hourOn_2, minOn_2
    global hourOn_3, minOn_3
    global ratio_on_1, ratio_off_1
    global ratio_on_2, ratio_off_2
    global ratio_on_3, ratio_off_3
    global dark_1, light_1
    global dark_2, light_2
    global dark_3, light_3

    hourOn_1 = []
    minOn_1=[]
    hourOn_2=[]
    minOn_2=[]
    hourOn_3=[]
    minOn_3=[]
    ratio_on_1=[]
    ratio_off_1=[]
    ratio_on_2=[]
    ratio_off_2=[]
    ratio_on_3=[]
    ratio_off_3=[]
    dark_1=[]
    light_1=[]
    dark_2=[]
    light_2=[]
    dark_3=[]
    light_3 = []

    starthour = spin0_1.get()
    startminute = spin0_2.get()
    startYear1_1 = year0_entry.get()
    startMonth1_1 = month0_entry.get()
    startdate1_1 = date0_entry.get()
    days1 = spin0_6.get()
    days2 = spin0_7.get()
    days3 = spin0_8.get()
    t_cycle1 = spin0_3.get()
    t_cycle2 = spin0_4.get()
    t_cycle3 = spin0_5.get()
    setBox=0

    for i in range(10):
        hourOn_1.append(spin_A_1[i].get())
        minOn_1.append(spin_B_1[i].get())
        ratio_on_1.append(spin_C_1[i].get())
        ratio_off_1.append(spin_D_1[i].get())
        if var_1[i].get() == 1:
            dark_1.append('0')
            light_1.append('0')
        if var_1[i].get() == 2:
            dark_1.append('1')
            light_1.append('0')
        if var_1[i].get() == 3:
            dark_1.append('0')
            light_1.append('1')
        hourOn_2.append(spin_A_2[i].get())
        minOn_2.append(spin_B_2[i].get())
        ratio_on_2.append(spin_C_2[i].get())
        ratio_off_2.append( spin_D_2[i].get())
        if var_2[i].get() == 1:
            dark_2.append('0')
            light_2.append('0')
        if var_2[i].get() == 2:
            dark_2.append('1')
            light_2.append('0')
        if var_2[i].get() == 3:
            dark_2.append('0')
            light_2.append('1')
        hourOn_3.append(spin_A_3[i].get())
        minOn_3.append(spin_B_3[i].get())
        ratio_on_3.append(spin_C_3[i].get())
        ratio_off_3.append(spin_D_3[i].get())
        if var_3[i].get() == 1:
            dark_3.append('0')
            light_3.append('0')
        if var_3[i].get() == 2:
            dark_3.append('1')
            light_3.append('0')
        if var_3[i].get() == 3:
            dark_3.append('0')
            light_3.append('1')
        setBox=setBox+1
    status.pack(side='bottom', fill='x')
    status.set('Schedules for all boxes are set.')
    # show_conf()
    btnSave['state'] = 'normal'
    btnRun['state'] = 'normal'
    recordingmenu.entryconfig('Start new', state='normal')
    window.update_idletasks()






if __name__ == '__main__':
    #### All of the components and their positions in the GUI ####
    # You can change the design from here #
    menu = Menu(window)  # define menu

    # Define Var to keep track of the schedule
    #1 for LD
    #2 for DD
    #3 for LL
    var_1= [IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1)]
    var_2= [IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1)]
    var_3= [IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1),IntVar(value=1)]

    #Create file menu
    filemenu = Menu(menu)
    filemenu.add_command(label='Load schedules')
    filemenu.add_command(label='Save schedules')
    filemenu.add_separator()
    filemenu.add_command(label='Quit', command=destruct)
    menu.add_cascade(label='File', menu=filemenu)
    #create setting menu
    settingmenu = Menu(menu)
    settingmenu.add_command(label='Set all boxes', command=getAllBoxSchedule)
    settingmenu.add_command(label='Show schedule')
    menu.add_cascade(label='Setting', menu=settingmenu)
    #create recording menu
    recordingmenu = Menu(menu)
    recordingmenu.add_command(label='Start new', command=connect)
    recordingmenu.entryconfig('Start new', state='disabled')
    #recordingmenu.add_command(label='Start revised', command=lambda:get_data(1))
    recordingmenu.add_separator()
    recordingmenu.add_command(label='Stop', command=disconnect)
    menu.add_cascade(label='Recording', menu=recordingmenu)
    #create About menu
    aboutmenu = Menu(menu)
    aboutmenu.add_command(label='About LocoBox', command=about)
    menu.add_cascade(label='Help', menu=aboutmenu)
    window.config(menu=menu)

    tab_control = ttk.Notebook(window)

    tab0 = ttk.Frame(tab_control)
    tab_control.add(tab0, text='T-cycles for all boxes')
    
    tab=[]
    for i in range(10):
        tab1 = ttk.Frame(tab_control)
        tab.append(tab1)
        tab_control.add(tab[i], text='Box%s'%i)

    tab11 = ttk.Frame(tab_control)
    tab_control.add(tab11, text='Schedules')

    #Display all available serial ports
    #openPorts=serial_ports()
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)
    openPorts = p.device
    if len(np.shape(openPorts)) == 0:
        openPorts = [openPorts]
    status.pack(side='bottom', fill='x')
    status.set('Available ports: '+', '.join(map(str, openPorts)))

    #Entry for Port, Baud, timeout, filename to save
    port = Label(text='Port').place(x=40, y=320)
    baud = Label(text='Baud rate').place(x=363, y=320)
    timeout = Label(text='Time out').place(x=565, y=320)
    filename = Label(text='File').place(x=40, y=360)
    configfilename = Label(text='Schedule').place(x=363, y=360)

    port_entry = Spinbox(values=openPorts, width=25)
    port_entry.delete(0, 'end')
    port_entry.insert(0, openPorts[0])  # first port is the default
    port_entry.place(x=80, y=320)
    baud_entry = Spinbox(values=(300, 600, 1200, 2400, 4800,
                                 9600, 14400, 19200, 28800, 38400, 57600, 115200), width=7)
    baud_entry.delete(0, 'end')
    baud_entry.insert(0, '9600')
    baud_entry.place(x=440, y=320)
    timeout_entry = Entry(width=4)
    timeout_entry.place(x=635, y=320)
    timeout_entry.insert(0, '10')
    filename_entry = Entry(width=25)
    filename_entry.place(x=80, y=360)
    # predefine a default filename with ISO date
    date_string = time.strftime('%Y%m%d')
    filename_entry.insert(0, 'BOX1-Tcycle-'+date_string+'.txt')
    configfilename_entry = Entry(width=25)
    configfilename_entry.place(x=440, y=360)
    configfilename_entry.insert(0, 'BOX1-sched-Tcycle-'+date_string+'.json')

    btnSave = Button(text=' Save ', state='disabled')
    btnRun = Button(text=' Recording Start ',
                    command=connect, state='disabled')

    # if box settings of all 10 boxes are done, activate save and run buttons
    if setBox == 10:
        btnSave['state'] = 'normal'
        btnRun['state'] = 'normal'
        recordingmenu.entryconfig('Start new', state='normal')
        # show_conf()
        window.update_idletasks()

    # button positions change depending on OS
    if sys.platform.startswith('win'):
        btnSave.place(x=610, y=360)
        btnRun.place(x=660, y=360)
    elif sys.platform.startswith('darwin'):
        btnSave.place(x=685, y=360)
        btnRun.place(x=745, y=360)
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        btnSave.place(x=610, y=360)
        btnRun.place(x=660, y=360)
    else:
        btnSave.place(x=685, y=360)
        btnRun.place(x=745, y=360)

    row_adj = 3  # useful when a new row is added above

    # T-cycle for all boxes
    fromLabel0 = Label(tab0, text='Start recording')
    space0 = Label(tab0, text='  ')
    spin0_1=Spinbox(tab0, from_=00, to=24, width=3, format='%02.0f')
    spin0_2=Spinbox(tab0, from_=00, to=24, width=3, format='%02.0f')
    spin0_1.delete(0, 'end')
    spin0_1.insert(0, '07')
    spin0_2.delete(0, 'end')
    spin0_2.insert(0, '00')
    label0_h0 = Label(tab0, text=':')
    date0_entry = Spinbox(tab0, from_=00, to=31, width=3, format='%02.0f')
    month0_entry = Spinbox(tab0, from_=00, to=12, width=3, format='%02.0f')
    year0_entry = Spinbox(tab0, from_=2018, to=2030, width=5)

        # calculate dates automatically for phase 1
    today = datetime.date.today() 
    date0_entry.delete(0, 'end')
    date0_entry.insert(0, '{:02d}'.format(today.day))
    month0_entry.delete(0, 'end')
    month0_entry.insert(0, '{:02d}'.format(today.month))
    year0_entry.delete(0, 'end')
    year0_entry.insert(0, today.year)  # ISO format is YYYY/MM/DD
    label0_d_0 = Label(tab0, text='/')
    label0_m_0 = Label(tab0, text='/')

    spin0_3 = Spinbox(tab0, from_=00, to=24, width=3, format='%02.0f')
    spin0_3.delete(0,'end')
    spin0_3.insert(0, '12')
    spin0_4 = Spinbox(tab0, from_=00, to=24, width=3, format='%02.0f')
    spin0_4.delete(0,'end')
    spin0_4.insert(0, '12')
    spin0_5 = Spinbox(tab0, from_=00, to=24, width=3, format='%02.0f')
    spin0_5.delete(0,'end')
    spin0_5.insert(0, '12')

    spin0_6 = Spinbox(tab0, from_=00, to=999, width=4, format='%03.0f')
    spin0_6.delete(0,'end')
    spin0_6.insert(0, '001')
    spin0_7 = Spinbox(tab0, from_=00, to=999, width=4, format='%03.0f')
    spin0_7.delete(0,'end')
    spin0_7.insert(0, '001')
    spin0_8 = Spinbox(tab0, from_=00, to=999, width=4, format='%03.0f')
    spin0_8.delete(0,'end')
    spin0_8.insert(0, '001')

    phaseLabel0_1 = Label(tab0, text='Phase 1:  ')
    phaseLabel0_2 = Label(tab0, text='Phase 2:  ')
    phaseLabel0_3 = Label(tab0, text='Phase 3:  ')
    T_label0_1 = Label(tab0, text='T-Cycle  ')
    T_label0_2 = Label(tab0, text='T-Cycle  ')
    T_label0_3 = Label(tab0, text='T-Cycle  ')
    T_labelend1 = Label(tab0, text='/24')
    T_labelend2 = Label(tab0, text='/24')
    T_labelend3 = Label(tab0, text='/24')
    DurationLabel1 = Label(tab0, text='Duration  ')
    DurationLabel2 = Label(tab0, text='Duration  ')
    DurationLabel3 = Label(tab0, text='Duration  ')
    DayLabel1 = Label(tab0, text='Days')
    DayLabel2 = Label(tab0, text='Days')
    DayLabel3 = Label(tab0, text='Days')


    fromLabel0.grid(column=0, row=2+row_adj, padx=15, pady=5)
    spin0_1.grid(column=1, row=2+row_adj)
    label0_h0.grid(column=2, row=2+row_adj)
    spin0_2.grid(column=3, row=2+row_adj)
    space0.grid(column=4, row=2+row_adj)
    year0_entry.grid(column=5, row=2+row_adj)
    label0_d_0.grid(column=6, row=2+row_adj)
    month0_entry.grid(column=7, row=2+row_adj)
    label0_m_0.grid(column=8, row=2+row_adj)
    date0_entry.grid(column=9, row=2+row_adj)

    phaseLabel0_1.grid(column=10, row=3+row_adj)
    T_label0_1.grid(column=11, row=3+row_adj)
    spin0_3.grid(column=12, row=3+row_adj)
    T_labelend1.grid(column=13, row=3+row_adj)
    DurationLabel1.grid(column=14, row=3+row_adj)
    spin0_6.grid(column=15, row=3+row_adj)
    DayLabel1.grid(column=16, row=3+row_adj)


    phaseLabel0_2.grid(column=10, row=4+row_adj)
    T_label0_2.grid(column=11, row=4+row_adj)
    spin0_4.grid(column=12, row=4+row_adj)
    T_labelend2.grid(column=13, row=4+row_adj)
    DurationLabel2.grid(column=14, row=4+row_adj)
    spin0_7.grid(column=15, row=4+row_adj)
    DayLabel2.grid(column=16, row=4+row_adj)

    phaseLabel0_3.grid(column=10, row=5+row_adj)
    T_label0_3.grid(column=11, row=5+row_adj)
    spin0_5.grid(column=12, row=5+row_adj)
    T_labelend3.grid(column=13, row=5+row_adj)
    DurationLabel3.grid(column=14, row=5+row_adj)
    spin0_8.grid(column=15, row=5+row_adj)
    DayLabel3.grid(column=16, row=5+row_adj)

    
    # Boxes
    boxes = [0,1,2,3,4,5,6,7,8,9]
    btn = []
    btnAll = []
    tab_title = []
    capSep = []
    boxsched_text  = []
    phaseLabel_1=[]

    rad_A_1 = []
    lbl_A_1 = []
    spin_A_1 = []
    boxsched_stat = []

    spin_B_1 = []
    label_h1_1 = []
    label_m1_1 = []
    lbl_B_1 = []
    spin_C_1 = []
    spin_D_1 = []
    label_h2_1 = []
    label_m2_1 = []
    rad_B_1 = []
    rad_C_1 = []

    phaseLabel_2 = []
    rad_A_2 = []
    lbl_A_2 = []
    spin_A_2 = []
    spin_B_2 = []
    label_h1_2 = []
    label_m1_2 = []
    lbl_B_2 = []
    spin_C_2 = []
    spin_D_2 = []
    label_h2_2 = []
    label_m2_2 = []
    rad_B_2 = []
    rad_C_2 = []

    phaseLabel_3 = []
    rad_A_3 = []
    lbl_A_3 = []
    spin_A_3 = []
    spin_B_3 = []
    spin_B_3 = []
    label_h1_3 = []
    label_m1_3 = []
    lbl_B_3 = []
    spin_C_3 = []
    spin_D_3 = []
    label_h2_3 = []
    label_m2_3 = []
    rad_B_3 = []
    rad_C_3 = []

    tab_title2 = []
    boxrec_text = []
    boxrec_stat = []

    for i in range(10):

        btnAll.append(Button(tab[i], text='Set All', command=getAllBoxSchedule))
        tab_title.append(Label(tab[i], text='LED schedule', anchor='center'))
        capSep.append(ttk.Separator(tab[i], orient=HORIZONTAL))
        boxsched_text.append(StringVar())
        phaseLabel_1.append(Label(tab[i], text='Phase 1'))
        rad_A_1.append(Radiobutton(
            tab[i], text='LD', variable=var_1[i], value=1))
        lbl_A_1.append(Label(tab[i], text='On:'))
        spin_A_1.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        boxsched_stat.append(Label(
            tab[i], textvariable=boxsched_text[i], anchor=W, justify=LEFT))
        spin_B_1.append(Spinbox(tab[i], from_=00, to=59, width=3, format='%02.0f'))
        label_h1_1.append(Label(tab[i], text=':'))
        label_m1_1.append(Label(tab[i], text=''))
        lbl_B_1.append(Label(tab[i], text='Ratio:'))
        spin_C_1.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        spin_D_1.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        label_h2_1.append(Label(tab[i], text=':'))
        label_m2_1.append(Label(tab[i], text=''))
        rad_B_1.append(Radiobutton(tab[i], text='DD', variable=var_1[i], value=2))
        rad_C_1.append(Radiobutton(tab[i], text='LL', variable=var_1[i], value=3))

        phaseLabel_2.append(Label(tab[i], text='Phase 2'))
        rad_A_2.append( Radiobutton(tab[i], text='LD', variable=var_2[i], value=1))
        lbl_A_2.append( Label(tab[i], text='On:'))
        spin_A_2.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))

        spin_B_2.append(Spinbox(tab[i], from_=00, to=59, width=3, format='%02.0f'))
        label_h1_2.append(Label(tab[i], text=':'))
        lbl_B_2.append(Label(tab[i], text='Ratio:'))
        spin_C_2.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        spin_D_2.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        label_h2_2.append(Label(tab[i], text=':'))
        label_m2_2.append( Label(tab[i], text=''))
        rad_B_2.append( Radiobutton(tab[i], text='DD', variable=var_2[i], value=2))
        rad_C_2.append(Radiobutton(
            tab[i], text='LL', variable=var_2[i], value=3))

        phaseLabel_3.append( Label(tab[i], text='Phase 3'))
        rad_A_3.append(Radiobutton(tab[i], text='LD', variable=var_3[i], value=1))
        lbl_A_3.append(Label(tab[i], text='On:'))
        spin_A_3.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        spin_B_3.append( Spinbox(tab[i], from_=00, to=59, width=3, format='%02.0f'))
        label_h1_3.append( Label(tab[i], text=':'))
        lbl_B_3.append(Label(tab[i], text='Ratio:'))
        spin_C_3.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        spin_D_3.append(Spinbox(tab[i], from_=00, to=24, width=3, format='%02.0f'))
        label_h2_3.append( Label(tab[i], text=':'))
        rad_B_3.append(Radiobutton(
            tab[i], text='DD', variable=var_3[i], value=2))
        rad_C_3.append(Radiobutton(tab[i], text='LL', variable=var_3[i], value=3))

        tab_title2.append(
            Label(tab[i], text='Recording status', anchor='center'))
        boxrec_text.append(StringVar())
        boxrec_stat.append(Label(tab[i], textvariable=boxrec_text[i],
                                 anchor='center', justify=LEFT))


    for i in boxes:
        boxsched_text[i].set('Schedule not set.')
        # phase 1
        spin_A_1[i].delete(0, 'end')
        spin_A_1[i].insert(0, '07')
        spin_B_1[i].delete(0, 'end')
        spin_B_1[i].insert(0, '00')

        spin_C_1[i].delete(0, 'end')
        spin_C_1[i].insert(0, '12')
        spin_D_1[i].delete(0, 'end')
        spin_D_1[i].insert(0, '24')

        phaseLabel_1[i].grid(column=0, row=1+row_adj, padx=15, pady=5)

        rad_A_1[i].grid(column=1, row=1+row_adj, pady=5)
        lbl_A_1[i].grid(column=2, row=1+row_adj, pady=5)
        spin_A_1[i].grid(column=3, row=1+row_adj, pady=5)
        label_h1_1[i].grid(column=4, row=1+row_adj, pady=5, sticky='w')
        spin_B_1[i].grid(column=5, row=1+row_adj, pady=5)
        lbl_B_1[i].grid(column=6, row=1+row_adj, pady=5)
        spin_C_1[i].grid(column=7, row=1+row_adj, pady=5)
        label_h2_1[i].grid(column=8, row=1+row_adj, pady=5, sticky='w')
        spin_D_1[i].grid(column=9, row=1+row_adj, pady=5)
        rad_B_1[i].grid(column=10, row=1+row_adj, padx=15, pady=5)
        rad_C_1[i].grid(column=11, row=1+row_adj, pady=5)

        # phase 2
        spin_A_2[i].delete(0, 'end')
        spin_A_2[i].insert(0, '07')
        spin_B_2[i].delete(0, 'end')
        spin_B_2[i].insert(0, '00')

        spin_C_2[i].delete(0, 'end')
        spin_C_2[i].insert(0, '12')
        spin_D_2[i].delete(0, 'end')
        spin_D_2[i].insert(0, '24')

        phaseLabel_2[i].grid(column=0, row=2+row_adj, padx=15, pady=5)

        rad_A_2[i].grid(column=1, row=2+row_adj, pady=5)
        lbl_A_2[i].grid(column=2, row=2+row_adj, pady=5)
        spin_A_2[i].grid(column=3, row=2+row_adj, pady=5)
        label_h1_2[i].grid(column=4, row=2+row_adj, pady=5)
        spin_B_2[i].grid(column=5, row=2+row_adj, pady=5)
        lbl_B_2[i].grid(column=6, row=2+row_adj, pady=5)
        spin_C_2[i].grid(column=7, row=2+row_adj, pady=5)
        label_h2_2[i].grid(column=8, row=2+row_adj, pady=5)
        spin_D_2[i].grid(column=9, row=2+row_adj, pady=5)
        rad_B_2[i].grid(column=10, row=2+row_adj, padx=15, pady=5)
        rad_C_2[i].grid(column=11, row=2+row_adj, pady=5)

        # phase 3
        spin_A_3[i].delete(0, 'end')
        spin_A_3[i].insert(0, '07')
        spin_B_3[i].delete(0, 'end')
        spin_B_3[i].insert(0, '00')
        spin_C_3[i].delete(0, 'end')
        spin_C_3[i].insert(0, '12')
        spin_D_3[i].delete(0, 'end')
        spin_D_3[i].insert(0, '24')

        phaseLabel_3[i].grid(column=0, row=3+row_adj, padx=15, pady=5)

        rad_A_3[i].grid(column=1, row=3+row_adj, pady=5)
        lbl_A_3[i].grid(column=2, row=3+row_adj, pady=5)
        spin_A_3[i].grid(column=3, row=3+row_adj, pady=5)
        label_h1_3[i].grid(column=4, row=3+row_adj, pady=5)
        spin_B_3[i].grid(column=5, row=3+row_adj, pady=5)
        lbl_B_3[i].grid(column=6, row=3+row_adj, pady=5)
        spin_C_3[i].grid(column=7, row=3+row_adj, pady=5)
        label_h2_3[i].grid(column=8, row=3+row_adj, pady=5)
        spin_D_3[i].grid(column=9, row=3+row_adj, pady=5)
        rad_B_3[i].grid(column=10, row=3+row_adj, padx=15, pady=5)
        rad_C_3[i].grid(column=11, row=3+row_adj, pady=5)

        ##Other stuffs
        btnAll[i].grid(column=1, row=4+row_adj, pady=5, columnspan='1', sticky='w')
        boxsched_stat[i].grid(column=3, row=4+row_adj, columnspan='8', sticky='w')
        

        tab_title2[i].grid(column=0, row=row_adj+6, columnspan='27', sticky='we')
        boxrec_text[i].set('Recording not started yet.')
        boxrec_stat[i].grid(column=0, row=row_adj+7, columnspan='27', sticky='we')
        tab_control.pack(expand=1, fill='both')
        window.update_idletasks()
        

    
    ### Main loop
    window.mainloop()
