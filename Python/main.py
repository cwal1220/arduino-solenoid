import sys
import os
import serial
import serial.tools.list_ports
import time
from PyQt5 import uic
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.uic import *
from PyQt5.QtGui import QPainter


## python실행파일 디렉토리
BASE_DIR = os.path.dirname(os.path.abspath(__file__)) 
Ui_MainWindow, QtBaseClass = uic.loadUiType(BASE_DIR + r'\MainWidget.ui')

class MainWidget(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        # uic.loadUi('MainWidget.ui', self)
        uic.loadUi(BASE_DIR + r'\MainWidget.ui', self)
        # self.ui = Ui_MainWindow()

        self.ser = None
        self.outMount = [0]
        self.serialReadThread = SerialReadThread(self)

        self.initButtonSignals()
        self.updateCOMPorts()
        

    def initButtonSignals(self):
        # Keypad
        self.pad1Button.clicked.connect(self.onPad1ButtonClicked)
        self.pad2Button.clicked.connect(self.onPad2ButtonClicked)
        self.pad3Button.clicked.connect(self.onPad3ButtonClicked)
        self.pad4Button.clicked.connect(self.onPad4ButtonClicked)
        self.pad5Button.clicked.connect(self.onPad5ButtonClicked)
        self.pad6Button.clicked.connect(self.onPad6ButtonClicked)
        self.pad7Button.clicked.connect(self.onPad7ButtonClicked)
        self.pad8Button.clicked.connect(self.onPad8ButtonClicked)
        self.pad9Button.clicked.connect(self.onPad9ButtonClicked)
        self.pad0Button.clicked.connect(self.onPad0ButtonClicked)
        self.padDeleteButton.clicked.connect(self.onPadDeleteButtonClicked)
        # Outlet select button
        self.setButton.clicked.connect(self.onSetButtonToggled)
        # Outlet extr button
        self.extrButton.clicked.connect(self.onExtrButtonToggled)
        # Outlet select button
        self.out1Button.toggled.connect(self.onOut1ButtonToggled)
        self.out2Button.toggled.connect(self.onOut2ButtonToggled)
        self.out3Button.toggled.connect(self.onOut3ButtonToggled)
        self.out4Button.toggled.connect(self.onOut4ButtonToggled)
        self.out5Button.toggled.connect(self.onOut5ButtonToggled)
        # COM Port ComboBox Selected
        self.comPortsComboBox.currentIndexChanged.connect(self.onComPortsComboBoxIndexChanged)

    def updateCOMPorts(self):
        self.comPortsComboBox.addItem("-") # Not Connected
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.comPortsComboBox.addItem(port.device)
        
    @pyqtSlot()
    def onPad1ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(1)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad2ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(2)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad3ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(3)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad4ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(4)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad5ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(5)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad6ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(6)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad7ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(7)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad8ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(8)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad9ButtonClicked(self):
        if len(self.outMount) <= 3:
            self.outMount.append(9)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPad0ButtonClicked(self):
        if 2 <= len(self.outMount) <= 3:
            self.outMount.append(0)
            result_to_int = int(''.join(map(str, self.outMount)))
            self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onPadDeleteButtonClicked(self):
        self.outMount = [0]
        result_to_int = int(''.join(map(str, self.outMount)))
        self.outSpinBox.setValue(result_to_int)

    @pyqtSlot()
    def onSetButtonToggled(self):
        # SET,1,홍길동,200,1.01 -> SET,1,OK,0
        # STAT,1 -> STAT,1,RUN,0
        # EXTR,1,300 -> EXTR,1,300,0(300)
        # CLEAN,1 -> CLEAN,1,OK
        if self.out1Button.isChecked():
            sendStr = 'SET,' + '1,' +  self.nameEdit.text() + ',' + str(self.outSpinBox.value()) + ',' + str(self.weightSpinBox.value()) + '\n'
        if self.out2Button.isChecked():
            sendStr = 'SET,' + '2,' +  self.nameEdit.text() + ',' + str(self.outSpinBox.value()) + ',' + str(self.weightSpinBox.value()) + '\n'
        if self.out3Button.isChecked():
            sendStr = 'SET,' + '3,' +  self.nameEdit.text() + ',' + str(self.outSpinBox.value()) + ',' + str(self.weightSpinBox.value()) + '\n'
        if self.out4Button.isChecked():
            sendStr = 'SET,' + '4,' +  self.nameEdit.text() + ',' + str(self.outSpinBox.value()) + ',' + str(self.weightSpinBox.value()) + '\n'
        if self.out5Button.isChecked():
            sendStr = 'SET,' + '5,' +  self.nameEdit.text() + ',' + str(self.outSpinBox.value()) + ',' + str(self.weightSpinBox.value()) + '\n'
        self.logEdit.append("[SEND] " + sendStr)
        self.logEdit.moveCursor(QTextCursor.End)
        self.ser.write(bytes(sendStr, 'utf-8'))

    @pyqtSlot()
    def onExtrButtonToggled(self):
        # SET,1,홍길동,200,1.01 -> SET,1,OK,0
        # STAT,1 -> STAT,1,RUN,0
        # EXTR,1,300 -> EXTR,1,300,0(300)
        # CLEAN,1 -> CLEAN,1,OK
        if self.out1Button.isChecked():
            sendStr = 'EXTR,' + '1,' + str(self.outSpinBox.value()) + '\n'
        if self.out2Button.isChecked():
            sendStr = 'EXTR,' + '2,' + str(self.outSpinBox.value()) + '\n'
        if self.out3Button.isChecked():
            sendStr = 'EXTR,' + '3,' + str(self.outSpinBox.value()) + '\n'
        if self.out4Button.isChecked():
            sendStr = 'EXTR,' + '4,' + str(self.outSpinBox.value()) + '\n'
        if self.out5Button.isChecked():
            sendStr = 'EXTR,' + '5,' + str(self.outSpinBox.value()) + '\n'
        self.logEdit.append("[SEND] " + sendStr)
        self.logEdit.moveCursor(QTextCursor.End)
        self.ser.write(bytes(sendStr, 'utf-8'))

    @pyqtSlot(bool)
    def onOut1ButtonToggled(self, toggled):
        if toggled:
            self.outletLabel.setText(self.out1Button.text())

    @pyqtSlot(bool)
    def onOut2ButtonToggled(self, toggled):
        if toggled:
            self.outletLabel.setText(self.out2Button.text())

    @pyqtSlot(bool)
    def onOut3ButtonToggled(self, toggled):
        if toggled:
            self.outletLabel.setText(self.out3Button.text())

    @pyqtSlot(bool)
    def onOut4ButtonToggled(self, toggled):
        if toggled:
            self.outletLabel.setText(self.out4Button.text())

    @pyqtSlot(bool)
    def onOut5ButtonToggled(self, toggled):
        if toggled:
            self.outletLabel.setText(self.out5Button.text())

    @pyqtSlot(int)
    def onComPortsComboBoxIndexChanged(self, idx):
        if self.ser != None:
            self.serialReadThread.terminateThead()
            self.ser.close()
            self.setButton.setEnabled(False)
            self.extrButton.setEnabled(False)
        if idx != 0:
            self.ser = serial.Serial(self.comPortsComboBox.currentText(), 9600)
            self.serialReadThread.start()
            self.setButton.setEnabled(True)
            self.extrButton.setEnabled(True)

class SerialReadThread(QThread):
    def __init__(self, parent): 
        super().__init__(parent)        
        self.parent = parent
        self.exitThread = False
        self.readLine = ''

    def run(self):
        while not self.exitThread:
            #데이터가 있있다면
            for c in self.parent.ser.read():
                if chr(c) == '\n':
                    self.parent.logEdit.append("[RECV] " + self.readLine)
                    self.parent.logEdit.moveCursor(QTextCursor.End)
                    self.readLine = ''
                else:
                    self.readLine += chr(c)

    def terminateThead(self):
        self.exitThread = True
        # 쓰레드 종료
        self.quit()
        self.wait(5000)


if __name__ == '__main__':
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    w = MainWidget()
    w.show()
    # w.showFullScreen()
    sys.exit(app.exec())





