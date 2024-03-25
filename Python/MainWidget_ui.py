# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'MainWidget.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *


class Ui_MainWidget(object):
    def setupUi(self, MainWidget):
        if not MainWidget.objectName():
            MainWidget.setObjectName(u"MainWidget")
        MainWidget.resize(515, 714)
        self.verticalLayout = QVBoxLayout(MainWidget)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.groupBox = QGroupBox(MainWidget)
        self.groupBox.setObjectName(u"groupBox")
        self.horizontalLayout = QHBoxLayout(self.groupBox)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.out1Button = QPushButton(self.groupBox)
        self.buttonGroup = QButtonGroup(MainWidget)
        self.buttonGroup.setObjectName(u"buttonGroup")
        self.buttonGroup.addButton(self.out1Button)
        self.out1Button.setObjectName(u"out1Button")
        sizePolicy = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.out1Button.sizePolicy().hasHeightForWidth())
        self.out1Button.setSizePolicy(sizePolicy)
        self.out1Button.setMinimumSize(QSize(80, 80))
        self.out1Button.setMaximumSize(QSize(80, 80))
        self.out1Button.setCheckable(True)
        self.out1Button.setChecked(True)

        self.horizontalLayout.addWidget(self.out1Button)

        self.out2Button = QPushButton(self.groupBox)
        self.buttonGroup.addButton(self.out2Button)
        self.out2Button.setObjectName(u"out2Button")
        sizePolicy.setHeightForWidth(self.out2Button.sizePolicy().hasHeightForWidth())
        self.out2Button.setSizePolicy(sizePolicy)
        self.out2Button.setMinimumSize(QSize(80, 80))
        self.out2Button.setMaximumSize(QSize(80, 80))
        self.out2Button.setCheckable(True)

        self.horizontalLayout.addWidget(self.out2Button)

        self.out3Button = QPushButton(self.groupBox)
        self.buttonGroup.addButton(self.out3Button)
        self.out3Button.setObjectName(u"out3Button")
        sizePolicy.setHeightForWidth(self.out3Button.sizePolicy().hasHeightForWidth())
        self.out3Button.setSizePolicy(sizePolicy)
        self.out3Button.setMinimumSize(QSize(80, 80))
        self.out3Button.setMaximumSize(QSize(80, 80))
        self.out3Button.setCheckable(True)

        self.horizontalLayout.addWidget(self.out3Button)

        self.out4Button = QPushButton(self.groupBox)
        self.buttonGroup.addButton(self.out4Button)
        self.out4Button.setObjectName(u"out4Button")
        sizePolicy.setHeightForWidth(self.out4Button.sizePolicy().hasHeightForWidth())
        self.out4Button.setSizePolicy(sizePolicy)
        self.out4Button.setMinimumSize(QSize(80, 80))
        self.out4Button.setMaximumSize(QSize(80, 80))
        self.out4Button.setCheckable(True)

        self.horizontalLayout.addWidget(self.out4Button)

        self.out5Button = QPushButton(self.groupBox)
        self.buttonGroup.addButton(self.out5Button)
        self.out5Button.setObjectName(u"out5Button")
        sizePolicy.setHeightForWidth(self.out5Button.sizePolicy().hasHeightForWidth())
        self.out5Button.setSizePolicy(sizePolicy)
        self.out5Button.setMinimumSize(QSize(80, 80))
        self.out5Button.setMaximumSize(QSize(80, 80))
        self.out5Button.setCheckable(True)

        self.horizontalLayout.addWidget(self.out5Button)


        self.verticalLayout.addWidget(self.groupBox)

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer)

        self.groupBox_2 = QGroupBox(MainWidget)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.gridLayout_2 = QGridLayout(self.groupBox_2)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.pad8Button = QPushButton(self.groupBox_2)
        self.pad8Button.setObjectName(u"pad8Button")
        sizePolicy.setHeightForWidth(self.pad8Button.sizePolicy().hasHeightForWidth())
        self.pad8Button.setSizePolicy(sizePolicy)
        self.pad8Button.setMinimumSize(QSize(80, 80))
        self.pad8Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad8Button, 2, 1, 1, 1)

        self.pad5Button = QPushButton(self.groupBox_2)
        self.pad5Button.setObjectName(u"pad5Button")
        sizePolicy.setHeightForWidth(self.pad5Button.sizePolicy().hasHeightForWidth())
        self.pad5Button.setSizePolicy(sizePolicy)
        self.pad5Button.setMinimumSize(QSize(80, 80))
        self.pad5Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad5Button, 1, 1, 1, 1)

        self.pad9Button = QPushButton(self.groupBox_2)
        self.pad9Button.setObjectName(u"pad9Button")
        sizePolicy.setHeightForWidth(self.pad9Button.sizePolicy().hasHeightForWidth())
        self.pad9Button.setSizePolicy(sizePolicy)
        self.pad9Button.setMinimumSize(QSize(80, 80))
        self.pad9Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad9Button, 2, 2, 1, 1)

        self.pad1Button = QPushButton(self.groupBox_2)
        self.pad1Button.setObjectName(u"pad1Button")
        sizePolicy.setHeightForWidth(self.pad1Button.sizePolicy().hasHeightForWidth())
        self.pad1Button.setSizePolicy(sizePolicy)
        self.pad1Button.setMinimumSize(QSize(80, 80))
        self.pad1Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad1Button, 0, 0, 1, 1)

        self.pad6Button = QPushButton(self.groupBox_2)
        self.pad6Button.setObjectName(u"pad6Button")
        sizePolicy.setHeightForWidth(self.pad6Button.sizePolicy().hasHeightForWidth())
        self.pad6Button.setSizePolicy(sizePolicy)
        self.pad6Button.setMinimumSize(QSize(80, 80))
        self.pad6Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad6Button, 1, 2, 1, 1)

        self.pad2Button = QPushButton(self.groupBox_2)
        self.pad2Button.setObjectName(u"pad2Button")
        sizePolicy.setHeightForWidth(self.pad2Button.sizePolicy().hasHeightForWidth())
        self.pad2Button.setSizePolicy(sizePolicy)
        self.pad2Button.setMinimumSize(QSize(80, 80))
        self.pad2Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad2Button, 0, 1, 1, 1)

        self.pad3Button = QPushButton(self.groupBox_2)
        self.pad3Button.setObjectName(u"pad3Button")
        sizePolicy.setHeightForWidth(self.pad3Button.sizePolicy().hasHeightForWidth())
        self.pad3Button.setSizePolicy(sizePolicy)
        self.pad3Button.setMinimumSize(QSize(80, 80))
        self.pad3Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad3Button, 0, 2, 1, 1)

        self.pad4Button = QPushButton(self.groupBox_2)
        self.pad4Button.setObjectName(u"pad4Button")
        sizePolicy.setHeightForWidth(self.pad4Button.sizePolicy().hasHeightForWidth())
        self.pad4Button.setSizePolicy(sizePolicy)
        self.pad4Button.setMinimumSize(QSize(80, 80))
        self.pad4Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad4Button, 1, 0, 1, 1)

        self.pad7Button = QPushButton(self.groupBox_2)
        self.pad7Button.setObjectName(u"pad7Button")
        sizePolicy.setHeightForWidth(self.pad7Button.sizePolicy().hasHeightForWidth())
        self.pad7Button.setSizePolicy(sizePolicy)
        self.pad7Button.setMinimumSize(QSize(80, 80))
        self.pad7Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad7Button, 2, 0, 1, 1)

        self.pad0Button = QPushButton(self.groupBox_2)
        self.pad0Button.setObjectName(u"pad0Button")
        sizePolicy.setHeightForWidth(self.pad0Button.sizePolicy().hasHeightForWidth())
        self.pad0Button.setSizePolicy(sizePolicy)
        self.pad0Button.setMinimumSize(QSize(80, 80))
        self.pad0Button.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.pad0Button, 3, 1, 1, 1)

        self.padDeleteButton = QPushButton(self.groupBox_2)
        self.padDeleteButton.setObjectName(u"padDeleteButton")
        sizePolicy.setHeightForWidth(self.padDeleteButton.sizePolicy().hasHeightForWidth())
        self.padDeleteButton.setSizePolicy(sizePolicy)
        self.padDeleteButton.setMinimumSize(QSize(80, 80))
        self.padDeleteButton.setMaximumSize(QSize(80, 80))

        self.gridLayout_2.addWidget(self.padDeleteButton, 3, 2, 1, 1)


        self.horizontalLayout_5.addWidget(self.groupBox_2)

        self.horizontalSpacer_2 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_2)


        self.verticalLayout.addLayout(self.horizontalLayout_5)

        self.groupBox_5 = QGroupBox(MainWidget)
        self.groupBox_5.setObjectName(u"groupBox_5")
        self.horizontalLayout_7 = QHBoxLayout(self.groupBox_5)
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.nameEdit = QLineEdit(self.groupBox_5)
        self.nameEdit.setObjectName(u"nameEdit")

        self.horizontalLayout_7.addWidget(self.nameEdit)


        self.verticalLayout.addWidget(self.groupBox_5)

        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.groupBox_3 = QGroupBox(MainWidget)
        self.groupBox_3.setObjectName(u"groupBox_3")
        sizePolicy1 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.groupBox_3.sizePolicy().hasHeightForWidth())
        self.groupBox_3.setSizePolicy(sizePolicy1)
        self.horizontalLayout_3 = QHBoxLayout(self.groupBox_3)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.outletLabel = QLabel(self.groupBox_3)
        self.outletLabel.setObjectName(u"outletLabel")
        font = QFont()
        font.setFamily(u"Consolas")
        font.setPointSize(18)
        self.outletLabel.setFont(font)
        self.outletLabel.setAlignment(Qt.AlignCenter)

        self.horizontalLayout_3.addWidget(self.outletLabel)


        self.horizontalLayout_4.addWidget(self.groupBox_3)

        self.groupBox_4 = QGroupBox(MainWidget)
        self.groupBox_4.setObjectName(u"groupBox_4")
        sizePolicy1.setHeightForWidth(self.groupBox_4.sizePolicy().hasHeightForWidth())
        self.groupBox_4.setSizePolicy(sizePolicy1)
        self.horizontalLayout_2 = QHBoxLayout(self.groupBox_4)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.outSpinBox = QSpinBox(self.groupBox_4)
        self.outSpinBox.setObjectName(u"outSpinBox")
        self.outSpinBox.setEnabled(True)
        self.outSpinBox.setMinimumSize(QSize(0, 80))
        self.outSpinBox.setFont(font)
        self.outSpinBox.setContextMenuPolicy(Qt.DefaultContextMenu)
        self.outSpinBox.setLayoutDirection(Qt.LeftToRight)
        self.outSpinBox.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)
        self.outSpinBox.setReadOnly(True)
        self.outSpinBox.setButtonSymbols(QAbstractSpinBox.NoButtons)
        self.outSpinBox.setMaximum(300)

        self.horizontalLayout_2.addWidget(self.outSpinBox)

        self.setButton = QPushButton(self.groupBox_4)
        self.setButton.setObjectName(u"setButton")
        self.setButton.setEnabled(False)
        sizePolicy.setHeightForWidth(self.setButton.sizePolicy().hasHeightForWidth())
        self.setButton.setSizePolicy(sizePolicy)
        self.setButton.setMinimumSize(QSize(80, 80))
        self.setButton.setMaximumSize(QSize(80, 80))

        self.horizontalLayout_2.addWidget(self.setButton)


        self.horizontalLayout_4.addWidget(self.groupBox_4)


        self.verticalLayout.addLayout(self.horizontalLayout_4)

        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.horizontalSpacer_3 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_3)

        self.comPortsComboBox = QComboBox(MainWidget)
        self.comPortsComboBox.setObjectName(u"comPortsComboBox")

        self.horizontalLayout_6.addWidget(self.comPortsComboBox)


        self.verticalLayout.addLayout(self.horizontalLayout_6)


        self.retranslateUi(MainWidget)

        QMetaObject.connectSlotsByName(MainWidget)
    # setupUi

    def retranslateUi(self, MainWidget):
        MainWidget.setWindowTitle(QCoreApplication.translate("MainWidget", u"Form", None))
        self.groupBox.setTitle(QCoreApplication.translate("MainWidget", u"\ud1a0\ucd9c\uad6c \uc120\ud0dd", None))
        self.out1Button.setText(QCoreApplication.translate("MainWidget", u"Outlet 1", None))
        self.out2Button.setText(QCoreApplication.translate("MainWidget", u"Outlet 2", None))
        self.out3Button.setText(QCoreApplication.translate("MainWidget", u"Outlet 3", None))
        self.out4Button.setText(QCoreApplication.translate("MainWidget", u"Outlet 4", None))
        self.out5Button.setText(QCoreApplication.translate("MainWidget", u"Outlet 5", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("MainWidget", u"\ud1a0\ucd9c \uc6a9\ub7c9 \uc120\ud0dd", None))
        self.pad8Button.setText(QCoreApplication.translate("MainWidget", u"8", None))
        self.pad5Button.setText(QCoreApplication.translate("MainWidget", u"5", None))
        self.pad9Button.setText(QCoreApplication.translate("MainWidget", u"9", None))
        self.pad1Button.setText(QCoreApplication.translate("MainWidget", u"1", None))
        self.pad6Button.setText(QCoreApplication.translate("MainWidget", u"6", None))
        self.pad2Button.setText(QCoreApplication.translate("MainWidget", u"2", None))
        self.pad3Button.setText(QCoreApplication.translate("MainWidget", u"3", None))
        self.pad4Button.setText(QCoreApplication.translate("MainWidget", u"4", None))
        self.pad7Button.setText(QCoreApplication.translate("MainWidget", u"7", None))
        self.pad0Button.setText(QCoreApplication.translate("MainWidget", u"0", None))
        self.padDeleteButton.setText(QCoreApplication.translate("MainWidget", u"Delete", None))
        self.groupBox_5.setTitle(QCoreApplication.translate("MainWidget", u"\uc0ac\uc6a9\uc790 \uc774\ub984", None))
        self.groupBox_3.setTitle(QCoreApplication.translate("MainWidget", u"\ud1a0\ucd9c\uad6c", None))
        self.outletLabel.setText(QCoreApplication.translate("MainWidget", u"Outlet 1", None))
        self.groupBox_4.setTitle(QCoreApplication.translate("MainWidget", u"\ud1a0\ucd9c\uc6a9\ub7c9", None))
        self.outSpinBox.setSuffix(QCoreApplication.translate("MainWidget", u" mL", None))
        self.setButton.setText(QCoreApplication.translate("MainWidget", u"Set", None))
    # retranslateUi

