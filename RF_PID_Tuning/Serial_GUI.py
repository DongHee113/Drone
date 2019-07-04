import sys
import Serial
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QComboBox, QPushButton, QLineEdit


class Serial_GUI(QWidget):

    Roll_Gain  = []
    Pitch_Gain = []
    Yaw_Gain   = []
    Alt_Gain   = []

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):

        # PID Gain Tuning Label
        self.label1 = QLabel('PID Gain Tuning', self)
        self.label1.move(110, 40)

        self.font1 = self.label1.font()
        self.font1.setPointSize(24)
        self.font1.setFamily('Agency FB')
        self.font1.setBold(True)

        self.label1.setFont(self.font1)

        # COM Label and ComboBox
        self.label2 = QLabel('COM', self)
        self.label2.move(390, 50)

        self.font2 = self.label2.font()
        self.font2.setPointSize(9)
        self.font2.setFamily('Gullim')

        self.label2.setFont(self.font2)
        self.COM = QComboBox(self)
        self.COM.move(370, 80)

        self.add_enable_COM()

        # Baudrate Label and ComboBox
        self.label3 = QLabel('Baudrate', self)
        self.label3.move(480, 50)

        self.font3 = self.label3.font()
        self.font3.setPointSize(9)
        self.font3.setFamily('Gullim')

        self.label2.setFont(self.font3)

        self.Baudrate = QComboBox(self)
        self.Baudrate.move(470, 80)

        self.Baudrate.addItem('9600')
        self.Baudrate.addItem('19200')
        self.Baudrate.addItem('38400')
        self.Baudrate.addItem('57600')
        self.Baudrate.addItem('115200')
        self.Baudrate.addItem('921600')

        # Transmit data ComboBox

        self.Transmit = QPushButton('Transmit', self)
        self.Transmit.clicked.connect(self.even_if_Transmit)
        self.Transmit.move(160, 520)
        self.Transmit.resize(281, 40)

        for i in range(0, 4):
            label = ['stab_P', 'rate_P', 'rate_I', 'rate_D']
            lblGainObj = QLabel(label[i], self)
            font = lblGainObj.font()
            font.setPointSize(12)
            font.setFamily('Agency Fb')
            font.setBold(True)

            lblGainObj.setFont(font)
            lblGainObj.resize(lblGainObj.sizeHint())
            lblGainObj.move(140 + 120 * i, 120)

        for i in range(0, 4):
            label = ['Roll', 'Pitch', 'Yaw', 'Alt']
            lblAxisObj = QLabel(label[i], self)
            font = lblAxisObj.font()
            font.setPointSize(12)
            font.setFamily('Agency Fb')
            font.setBold(True)

            lblAxisObj.setFont(font)
            lblAxisObj.resize(lblAxisObj.sizeHint())
            lblAxisObj.move(40, 190 + 90 * i)

        for i in range(0, 4):
            RollGainObj = QLineEdit(self)
            RollGainObj.resize(103, 30)
            RollGainObj.move(110 + 120 * i, 190)
            self.Roll_Gain.append(RollGainObj)

        for i in range(0, 4):
            PitchGainObj = QLineEdit(self)
            PitchGainObj.resize(103, 30)
            PitchGainObj.move(110 + 120 * i, 280)
            self.Pitch_Gain.append(PitchGainObj)

        for i in range(0, 4):
            YawGainObj = QLineEdit(self)
            YawGainObj.resize(103, 30)
            YawGainObj.move(110 + 120 * i, 370)
            self.Yaw_Gain.append(YawGainObj)

        for i in range(0, 3):
            AltGainObj = QLineEdit(self)
            AltGainObj.resize(103, 30)
            AltGainObj.move(230 + 120 * i, 460)
            self.Alt_Gain.append(AltGainObj)

        self.setWindowTitle('PID Gain Tuning')
        self.move(300, 300)
        self.resize(600, 600)
        self.show()

    def add_enable_COM(self):
        self.enableCOM = Serial.findEnableCOMPort()
        for ser in self.enableCOM:
            self.COM.addItem(ser)

    def even_if_Transmit(self):
        txtGain = [self.Roll_Gain, self.Pitch_Gain, self.Yaw_Gain]
        gainType = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O']
        gainTypeIndex = 0;

        serial = self.COM.currentText()
        baudrate = int(self.Baudrate.currentText())

        for gain in txtGain:
            for gainTermIndex in range(0, 4):
                if gain[gainTermIndex].text() != '' and gain[gainTermIndex].isEnabled():
                    Serial.transmit_packet(serial, baudrate, gainType[gainTypeIndex], float(gain[gainTermIndex].text()))
                gainTypeIndex += 1

        for idx in range(0, 3):
            if self.Alt_Gain[idx].text() != '' and self.Alt_Gain[idx].isEnabled():
                Serial.transmit_packet(serial, baudrate, gainType[gainTypeIndex], float(self.Alt_Gain[idx].text()))
            gainTypeIndex += 1
