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
        # self.setWindowFlag(Qt.FramelessWindowHint)
        # self.showFullScreen()
        
        # timer = QTimer(self)
        # timer.timeout.connect(self.updateChart)
        # timer.start(40)
        # self.initGraph()



if __name__ == '__main__':
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    w = MainWidget()
    w.show()
    # w.showFullScreen()
    sys.exit(app.exec())
