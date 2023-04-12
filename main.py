import sys
import os
from PyQt5 import uic
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.uic import *
from PyQt5.QtGui import QPainter


class MainWidget(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        uic.loadUi('MainWidget.ui', self)
        self.initButtonSignals()

        self.outMount = [0]
        # self.setWindowFlag(Qt.FramelessWindowHint)
        # self.showFullScreen()
        
        # timer = QTimer(self)
        # timer.timeout.connect(self.updateChart)
        # timer.start(40)
        # self.initGraph()

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
        self.setButton.toggled.connect(self.onSetButtonToggled)
        # Outlet select button
        self.out1Button.toggled.connect(self.onOut1ButtonToggled)
        self.out2Button.toggled.connect(self.onOut2ButtonToggled)
        self.out3Button.toggled.connect(self.onOut3ButtonToggled)
        self.out4Button.toggled.connect(self.onOut4ButtonToggled)
        self.out5Button.toggled.connect(self.onOut5ButtonToggled)

        
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
        # TODO: Send data to Arduino
        pass

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



if __name__ == '__main__':
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    w = MainWidget()
    w.show()
    # w.showFullScreen()
    sys.exit(app.exec())
