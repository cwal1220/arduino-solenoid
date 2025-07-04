import sys
import os
import struct
from PyQt5 import uic
from PyQt5.QtCore import QTimer, pyqtSlot
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.QtGui import QTextCursor
from pymodbus.client.sync import ModbusSerialClient as ModbusClient
import serial.tools.list_ports

# --- 기본 설정 ---
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
Ui_MainWidget, _ = uic.loadUiType(os.path.join(BASE_DIR, 'MainWidget.ui'))

# --- Modbus 상수 (레지스터 블록 내 오프셋) ---
REGS_PER_OUTLET = 28
OFFSET_CMD = 0
OFFSET_STATUS = 1
OFFSET_DOSE = 2
OFFSET_WEIGHT = 3
OFFSET_USER_NAME = 4
OFFSET_DRUG_NAME = 16

STATUS_MAP = {
    0: "정지", 1: "추출대기", 2: "추출중", 3: "추출완료", 4: "수동추출",
}

class MainWidget(QWidget, Ui_MainWidget):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.modbus_client = None
        self.current_outlet_id = 1  # 기본 선택은 1번 토출구
        self.last_status = -1
        self.outMount = [0]

        self._init_signals()
        self._update_com_ports()

        self.status_timer = QTimer(self)
        self.status_timer.timeout.connect(self.poll_status)
        self.status_timer.start(50)

    def _init_signals(self):
        # ... (기존 시그널 연결과 동일)
        self.comPortsComboBox.currentIndexChanged.connect(self.on_com_port_changed)
        self.setButton.clicked.connect(self.on_set_button_clicked)
        self.extrButton.clicked.connect(self.on_clean_button_clicked)
        # ... (키패드, 토출구 버튼 시그널 연결)
        self.pad1Button.clicked.connect(self.on_pad1_button_clicked)
        self.pad2Button.clicked.connect(self.on_pad2_button_clicked)
        self.pad3Button.clicked.connect(self.on_pad3_button_clicked)
        self.pad4Button.clicked.connect(self.on_pad4_button_clicked)
        self.pad5Button.clicked.connect(self.on_pad5_button_clicked)
        self.pad6Button.clicked.connect(self.on_pad6_button_clicked)
        self.pad7Button.clicked.connect(self.on_pad7_button_clicked)
        self.pad8Button.clicked.connect(self.on_pad8_button_clicked)
        self.pad9Button.clicked.connect(self.on_pad9_button_clicked)
        self.pad0Button.clicked.connect(self.on_pad0_button_clicked)
        self.padDeleteButton.clicked.connect(self.on_pad_delete_button_clicked)

        self.out1Button.toggled.connect(self.on_out1_button_toggled)
        self.out2Button.toggled.connect(self.on_out2_button_toggled)
        self.out3Button.toggled.connect(self.on_out3_button_toggled)
        self.out4Button.toggled.connect(self.on_out4_button_toggled)
        self.out5Button.toggled.connect(self.on_out5_button_toggled)


    def _get_base_addr(self):
        """현재 선택된 토출구의 Modbus 시작 주소를 계산합니다."""
        return (self.current_outlet_id - 1) * REGS_PER_OUTLET

    def poll_status(self):
        if not (self.modbus_client and self.modbus_client.is_socket_open()):
            return
        try:
            status_addr = self._get_base_addr() + OFFSET_STATUS
            rr = self.modbus_client.read_holding_registers(status_addr, 1, unit=1)
            if not rr.isError():
                current_status_code = rr.registers[0]
                if current_status_code != self.last_status:
                    status_str = STATUS_MAP.get(current_status_code, "알 수 없음")
                    self._log(f"[OUTLET {self.current_outlet_id} STATUS] 상태 변경: {status_str} ({current_status_code})")
                    self.last_status = current_status_code
        except Exception as e:
            self._log(f"[ERROR] 상태 읽기 실패: {e}")

    @pyqtSlot()
    def on_set_button_clicked(self):
        if not (self.modbus_client and self.modbus_client.is_socket_open()):
            return
        try:
            base_addr = self._get_base_addr()
            cmd = 1
            dose = self.outSpinBox.value()
            weight = self.weightSpinBox.value()
            user_name = self.nameEdit.text()
            drug_name = self.drugEdit.text()

            # 주소 계산
            cmd_addr = base_addr + OFFSET_CMD
            dose_addr = base_addr + OFFSET_DOSE
            weight_addr = base_addr + OFFSET_WEIGHT
            user_name_addr = base_addr + OFFSET_USER_NAME
            drug_name_addr = base_addr + OFFSET_DRUG_NAME

            # 데이터 쓰기
            self.modbus_client.write_register(dose_addr, dose, unit=1)
            self.modbus_client.write_register(weight_addr, weight, unit=1)
            
            user_name_bytes = user_name.encode('utf-8').ljust(24, b'\x00')
            user_name_regs = list(struct.unpack('>12H', user_name_bytes))
            self.modbus_client.write_registers(user_name_addr, user_name_regs, unit=1)

            drug_name_bytes = drug_name.encode('utf-8').ljust(24, b'\x00')
            drug_name_regs = list(struct.unpack('>12H', drug_name_bytes))
            self.modbus_client.write_registers(drug_name_addr, drug_name_regs, unit=1)

            # Important!!!! cmd는 반드시 제일 나중에 보내기.
            self.modbus_client.write_register(cmd_addr, cmd, unit=1)

            self._log(f"[OUTLET {self.current_outlet_id} SEND] 약물 셋팅 완료")

            # --- 전송 후 레지스터 다시 읽기 및 로그 출력 ---
            self._log(f"[OUTLET {self.current_outlet_id} VERIFY] 전송된 값:")
            
            # 전체 블록을 다시 읽어옴
            rr_read = self.modbus_client.read_holding_registers(base_addr, REGS_PER_OUTLET, unit=1)
            if not rr_read.isError():
                regs_read = rr_read.registers

                read_dose = regs_read[OFFSET_DOSE]
                read_weight = regs_read[OFFSET_WEIGHT]
                
                read_user_name_bytes = struct.pack('>12H', *regs_read[OFFSET_USER_NAME:OFFSET_USER_NAME+12])
                read_user_name = read_user_name_bytes.split(b'\x00', 1)[0].decode('utf-8', errors='ignore')

                read_drug_name_bytes = struct.pack('>12H', *regs_read[OFFSET_DRUG_NAME:OFFSET_DRUG_NAME+12])
                read_drug_name = read_drug_name_bytes.split(b'\x00', 1)[0].decode('utf-8', errors='ignore')

                self._log(f"  - Dose: {read_dose}")
                self._log(f"  - Weight: {read_weight}")
                self._log(f"  - User Name: '{read_user_name}'")
                self._log(f"  - Drug Name: '{read_drug_name}'")
            else:
                self._log(f"  [ERROR] 레지스터 읽기 실패: {rr_read}")

        except Exception as e:
            self._log(f"[ERROR] 약물 셋팅 실패: {e}")

    @pyqtSlot()
    def on_clean_button_clicked(self):
        if not (self.modbus_client and self.modbus_client.is_socket_open()):
            return
        try:
            cmd_addr = self._get_base_addr() + OFFSET_CMD
            self.modbus_client.write_register(cmd_addr, 2, unit=1)
            self._log(f"[OUTLET {self.current_outlet_id} SEND] 청소 명령 전송")
        except Exception as e:
            self._log(f"[ERROR] 청소 명령 실패: {e}")

    def _set_current_outlet(self, outlet_id, outlet_text):
        """현재 토출구를 설정하고 UI를 업데이트합니다."""
        self.current_outlet_id = outlet_id
        self.outletLabel.setText(outlet_text)
        self.last_status = -1 # 상태 강제 업데이트를 위해 초기화
        self._log(f"--- {outlet_text} 선택됨 ---")

    @pyqtSlot(bool)
    def on_out1_button_toggled(self, toggled):
        if toggled: self._set_current_outlet(1, self.out1Button.text())
    @pyqtSlot(bool)
    def on_out2_button_toggled(self, toggled):
        if toggled: self._set_current_outlet(2, self.out2Button.text())
    @pyqtSlot(bool)
    def on_out3_button_toggled(self, toggled):
        if toggled: self._set_current_outlet(3, self.out3Button.text())
    @pyqtSlot(bool)
    def on_out4_button_toggled(self, toggled):
        if toggled: self._set_current_outlet(4, self.out4Button.text())
    @pyqtSlot(bool)
    def on_out5_button_toggled(self, toggled):
        if toggled: self._set_current_outlet(5, self.out5Button.text())

    # --- 기존 UI 및 통신 관련 메서드들 (큰 변경 없음) ---
    # (on_com_port_changed, _update_com_ports, _log, 키패드 관련 슬롯 등)
    # ... (이하 생략, 기존 코드와 거의 동일)
    def _update_com_ports(self):
        self.comPortsComboBox.addItem("-")
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.comPortsComboBox.addItem(port.device)

    def _log(self, message):
        self.logEdit.append(message)
        self.logEdit.moveCursor(QTextCursor.End)

    def _update_dose_display(self):
        if not self.outMount:
            self.outMount = [0]
        result_to_int = int(''.join(map(str, self.outMount)))
        self.outSpinBox.setValue(result_to_int)
        
    @pyqtSlot(int)
    def on_com_port_changed(self, index):
        if self.modbus_client and self.modbus_client.is_socket_open():
            self.modbus_client.close()
            self._log("[INFO] Modbus 연결 해제")

        port_name = self.comPortsComboBox.itemText(index)
        if port_name != "-":
            self.modbus_client = ModbusClient(
                method='rtu', port=port_name, baudrate=9600,
                stopbits=1, bytesize=8, parity='N', timeout=0.5
            )
            if self.modbus_client.connect():
                self._log(f"[INFO] Modbus 연결 성공: {port_name}")
                self.setButton.setEnabled(True)
                self.extrButton.setEnabled(True)
            else:
                self._log(f"[ERROR] Modbus 연결 실패: {port_name}")
                self.setButton.setEnabled(False)
                self.extrButton.setEnabled(False)

    def _append_dose(self, num):
        if len(self.outMount) <= 3:
            if self.outMount == [0]: self.outMount = []
            self.outMount.append(num)
            self._update_dose_display()

    @pyqtSlot()
    def on_pad1_button_clicked(self): self._append_dose(1)
    @pyqtSlot()
    def on_pad2_button_clicked(self): self._append_dose(2)
    @pyqtSlot()
    def on_pad3_button_clicked(self): self._append_dose(3)
    @pyqtSlot()
    def on_pad4_button_clicked(self): self._append_dose(4)
    @pyqtSlot()
    def on_pad5_button_clicked(self): self._append_dose(5)
    @pyqtSlot()
    def on_pad6_button_clicked(self): self._append_dose(6)
    @pyqtSlot()
    def on_pad7_button_clicked(self): self._append_dose(7)
    @pyqtSlot()
    def on_pad8_button_clicked(self): self._append_dose(8)
    @pyqtSlot()
    def on_pad9_button_clicked(self): self._append_dose(9)
    @pyqtSlot()
    def on_pad0_button_clicked(self):
        if self.outMount and self.outMount != [0] and len(self.outMount) <= 3:
            self.outMount.append(0)
            self._update_dose_display()

    @pyqtSlot()
    def on_pad_delete_button_clicked(self):
        self.outMount = [0]
        self._update_dose_display()

    def closeEvent(self, event):
        if self.modbus_client and self.modbus_client.is_socket_open():
            self.modbus_client.close()
        event.accept()


if __name__ == '__main__':
    import signal
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    w = MainWidget()
    w.show()
    sys.exit(app.exec())
