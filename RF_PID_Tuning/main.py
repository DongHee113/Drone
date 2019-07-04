from PyQt5.QtWidgets import QApplication
from Serial_GUI import Serial_GUI

if __name__ == '__main__':
    def run():
        app = QApplication([])
        GUI = Serial_GUI()
        app.exec_()

    run()
