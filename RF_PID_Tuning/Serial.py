from ctypes import *
from PyQt5.QtTest import QTest
import serial


def findEnableCOMPort():
    ser = []
    for i in range(0, 100):
        try:
            serObj = serial.Serial(
                port='COM' + str(i),
                baudrate=9600,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                bytesize=serial.EIGHTBITS,
                timeout=1)
            ser.append(serObj.port)
            serObj.close()
        except serial.serialutil.SerialException:
            pass
    return ser


def transmit_packet(portNum, baudRate, gainType, gainValue):

    send_str = '\x02' + gainType + format(gainValue, '07.3f') + '\x03'

    try:
        ser = serial.Serial(
            port=portNum,
            baudrate=baudRate,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=0)
    except:
        print("error")

    print(send_str)

    ser.write(bytes(send_str.encode()))

    QTest.qWait(5)
