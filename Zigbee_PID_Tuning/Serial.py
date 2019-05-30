from ctypes import *
from PyQt5.QtTest import QTest
import serial

ser = []
packet = []

class uint8_arr(Array):
    _type_ = c_uint8
    _length_ = 4


class union(Union):
    _fields_ = ("data", c_float), ("buff", uint8_arr)


def findEnableCOMPort():
    global ser
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


def makePacket(gainType, gainValue):
    checksum = 0
    gainUnion = union()
    gainUnion.data = gainValue


    #packet.append(83)
    #packet.append(serial.to_bytes([0x53]))
    packet.append(0x00)
    packet.append(0x01)
    packet.append(gainType)
    packet.append(gainUnion.buff[0])
    packet.append(gainUnion.buff[1])
    packet.append(gainUnion.buff[2])
    packet.append(gainUnion.buff[3])
    packet.append(0xFF)

    # calculate checksum
    for data in packet:
        checksum += data

    checksum = 0x0100 - (checksum & 0x00FF)
    packet.append(checksum)


def transmit_packet(portNum, baudRate, gainType, gainValue):
    makePacket(gainType, gainValue)

    try:
        ser = serial.Serial(
            port=portNum,\
            baudrate=baudRate,\
            parity=serial.PARITY_NONE,\
            stopbits=serial.STOPBITS_ONE,\
            bytesize=serial.EIGHTBITS,\
            timeout = 0)
    except:
        pass

    for data in packet:
        print(format(data, '#04x'), end=' ')
        ser.write(c_uint8(data))
        QTest.qWait(10)

    QTest.qWait(50)

    print('')
    packet.clear()

