# 기본
import sys
import json
import ast
import re
import os
# PyQt 관련
from PyQt5 import QtWidgets, uic
from PyQt5.QtCore import QTimer, QThread
from PyQt5.QtGui import QTextCursor
# Serial 관련
import serial
import serial.tools.list_ports
# Groq 관련
import groq

# Groq API 키 설정
client = groq.Groq(api_key="gsk_xxx")  # 개인 키로 교체

## python실행파일 디렉토리
BASE_DIR = os.path.dirname(os.path.abspath(__file__)) 
Ui_MainWindow, QtBaseClass = uic.loadUiType(BASE_DIR + r'\drink_recommend.ui')

DRINKS = [
    "프로틴",
    "마그네슘",
    "전해질",
    "프리-워크아웃",
    "칼로리버너"
]

def clean_and_parse_json(text):
    """
    입력 text에서 마크다운 코드펜스 ```...``` 를 제거하고,
    처음 나오는 '{'와 마지막 '}' 사이를 잘라 JSON으로 파싱합니다.
    - 성공하면 dict 반환
    - 실패하면 예외 발생
    """
    if not isinstance(text, str):
        raise ValueError("input must be str")

    txt = text.strip()

    # 1) 코드펜스 제거 (``` 또는 ```json 같은 경우)
    #    ``` 로 시작하면 처음과 마지막 ```를 제거
    if txt.startswith("```"):
        # remove leading fence line (``` or ```json)
        txt = re.sub(r"^```[^\n]*\n", "", txt)
        # remove trailing fence if present
        txt = re.sub(r"\n```$", "", txt).strip()

    # 2) 혹시 마크다운 인라인 ```가 아니라도, 전체 문자열에서 JSON 오브젝트 부분만 추출
    start = txt.find("{")
    end = txt.rfind("}")
    if start == -1 or end == -1 or end < start:
        raise ValueError("JSON object not found in the input")

    body = txt[start:end+1]

    # 3) 스마트 따옴표(‘’ “”)를 일반 따옴표로 교정 (옵션)
    body = body.replace("“", '"').replace("”", '"').replace("‘", "'").replace("’", "'")

    # 4) json.loads 시도
    try:
        return json.loads(body)
    except json.JSONDecodeError:
        # 작은따옴표 사용 등으로 실패하면 ast.literal_eval 시도
        try:
            return ast.literal_eval(body)
        except Exception as e:
            # 최종 실패: 원본 body와 함께 예외 던짐
            raise ValueError(f"파싱 실패: {e}\n원본 추출 텍스트:\n{body}")

def ask_groq_for_drink(data):
    # 모델에게 꼭 JSON object를 반환하도록 더 명확히 요구
    prompt = f"""
사용자 정보:
- 오늘 활동량: {data['activity']}
- 무산소 운동 강도: {data['anaerobic']}
- 유산소 운동 강도: {data['aerobic']}
- 땀 흘린 양: {data['sweat']}
- 키: {data['height']} cm
- 몸무게: {data['weight']} kg
- 감량 목표: {data['goal']} kg
- 성별: {data['gender']}
- 나이: {data['age']}세

아래 5개의 음료에 대해 권장 섭취량(단위: ml)을 결정하세요.
또한 권장 이유를 '이유' 필드에 하나의 문자열로 자세히 적어주세요.
출력은 반드시 JSON object 형태로, 예시는 정확히 아래 형식과 같아야 합니다.

필드는 아래와 같이 구성하세요.
프로틴: 용량
마그네슘: 용량
전해질: 용량
프리-워크아웃: 용량
칼로리버너: 용량
이유: 사용자 입력(예: 무산소 8/10, 땀 많음 등)을 언급하며 권장 근거를 자세히 서술

응답은 오직 JSON만 출력하세요(추가 텍스트 금지).
"""

    try:
        chat_completion = client.chat.completions.create(
            model="llama-3.3-70b-versatile",
            messages=[
                {"role": "system", "content": "운동 후 적절한 음료와 섭취량을 추천하는 건강 전문가입니다. 출력은 반드시 JSON 객체만 하세요."},
                {"role": "user", "content": prompt}
            ],
            temperature=0.2,
        )

        raw = chat_completion.choices[0].message.content.strip()
        parsed = clean_and_parse_json(raw)
        if parsed is None or not isinstance(parsed, dict):
            return None, f"응답 파싱 실패: 모델 응답을 JSON으로 변환할 수 없습니다.\n원본 응답: {raw}"

        # 유효성 검사: 필수 키들 존재하는지 확인
        missing = [d for d in DRINKS if d not in parsed]
        if missing:
            return None, f"응답에 다음 음료 필드가 없습니다: {missing}. 응답: {parsed}"

        reason = parsed.get("이유") or parsed.get("reason") or ""
        # 음료별 권장량 추출(정수로 변환 시도)
        volumes = {}
        for d in DRINKS:
            try:
                volumes[d] = int(parsed[d])
            except Exception:
                # 숫자 변환 실패하면 원본값 유지(문자열)
                volumes[d] = parsed[d]

        return {"volumes": volumes, "reason": reason}, None

    except Exception as e:
        return None, f"Groq API 오류: {e}"

class DrinkRecommendationApp(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        # uic.loadUi("drink_recommend.ui", self)
        uic.loadUi(BASE_DIR + r'\drink_recommend.ui', self)
        # 버튼 연결
        self.btnRecommend.clicked.connect(self.on_recommend)
        self.btnExtract.clicked.connect(self.on_extract)
        self.btnReturn.clicked.connect(self.on_return)
        # COM Port ComboBox Selected
        self.comPortsComboBox.currentIndexChanged.connect(self.onComPortsComboBoxIndexChanged)

        # 타이핑 관련 초기값
        self._typing_timer = None
        self._typing_index = 0
        self._typing_text = ""

        self.ser = None
        self.serialReadThread = SerialReadThread(self)
        self.updateCOMPorts()

    # 타이핑 이벤트
    def type_text(self, text, on_finished=None, speed_ms=20):
        # 타이핑 시작 전에 기존 타이머 정리
        if hasattr(self, "_typing_timer") and self._typing_timer is not None:
            try:
                self._typing_timer.stop()
            except Exception:
                pass

        self.textResult.clear()
        self._typing_index = 0
        # 문자열이 아닌 경우 문자열로 변환
        self._typing_text = str(text)
        self._typing_finished_callback = on_finished
        self._typing_timer = QTimer(self)
        self._typing_timer.timeout.connect(self._update_typing_text)
        self._typing_timer.start(speed_ms)

    def updateCOMPorts(self):
        self.comPortsComboBox.addItem("-") # Not Connected
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.comPortsComboBox.addItem(port.device)

    # 타이핑 이벤트 내부
    def _update_typing_text(self):
        if self._typing_index < len(self._typing_text):
            # 한 글자씩 추가
            # toPlainText -> 기존 내용 얻어와 append 하면 느릴 수 있으므로 setPlainText에 슬라이스도 가능
            current = self._typing_text[: self._typing_index + 1]
            self.textResult.setPlainText(current)
            self.textResult.moveCursor(QTextCursor.End)
            self._typing_index += 1
        else:
            # 종료
            if self._typing_timer:
                self._typing_timer.stop()
            # 버튼 활성화 콜백
            self.btnRecommend.setEnabled(True)
            if callable(getattr(self, "_typing_finished_callback", None)):
                try:
                    self._typing_finished_callback()
                except Exception:
                    pass

    def get_selected_radio(self, prefix):
        # __dict__에서 radio 버튼 찾아서 체크된 텍스트 반환
        for name, obj in self.__dict__.items():
            if name.startswith(prefix) and isinstance(obj, QtWidgets.QRadioButton) and obj.isChecked():
                return obj.text()
        return ""

    def on_recommend(self):
        # 버튼 비활성화
        self.btnRecommend.setEnabled(False)

        activity = self.get_selected_radio("radioActivity")
        anaerobic = self.get_selected_radio("radioAnaerobic")
        aerobic = self.get_selected_radio("radioAerobic")
        sweat = self.get_selected_radio("radioSweat")
        gender = self.get_selected_radio("radioGender")

        data = {
            "activity": activity,
            "anaerobic": anaerobic,
            "aerobic": aerobic,
            "sweat": sweat,
            "height": self.spinHeight.value(),
            "weight": self.spinWeight.value(),
            "goal": self.spinGoal.value(),
            "gender": gender,
            "age": self.spinAge.value()
        }
        print("request data:", data)
        self.textProgress.setPlainText("생각하는 중...💭 잠시만 기다려 주세요.")
        QtWidgets.QApplication.processEvents()
        self.stackedWidget.setCurrentIndex(1)

        # 모델 호출 (동기)
        result, err = ask_groq_for_drink(data)
        if err:
            # 타이핑 효과로 에러 메시지 출력
            self.type_text(err)
            return

        # 정상 응답: result는 {'volumes': {...}, 'reason': "..."}
        volumes = result["volumes"]
        reason = result["reason"]

        # SpinBox에 추출량 출력
        self.spinProtein.setValue(volumes['프로틴'])
        self.spinMg.setValue(volumes['마그네슘'])
        self.spinElect.setValue(volumes['전해질'])
        self.spinFree.setValue(volumes['프리-워크아웃'])
        self.spinCal.setValue(volumes['칼로리버너'])

        # textResult에 이유 출력 (타자효과)
        self.type_text(reason)

    def on_extract(self):
        # SET,1,홍길동,타이레놀,200,1.01 -> SET,1,OK,0
        # STAT,1 -> STAT,1,RUN,0
        # EXTR,1,300 -> EXTR,1,300,0(300)
        # CLEAN,1 -> CLEAN,1,OK

        sendStr = 'SET,' + '1,' +  '프로틴' + ',' + '프로틴' + ',' + str(self.spinProtein.value()) + ',' + '1.0' + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'SET,' + '2,' +  '마그네슘' + ',' + '마그네슘' + ',' + str(self.spinMg.value()) + ',' + '1.0' + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'SET,' + '3,' +  '전해질' + ',' + '전해질' + ',' + str(self.spinElect.value()) + ',' + '1.0' + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'SET,' + '4,' +  '프리-워크아웃' + ',' + '프리-워크아웃' + ',' + str(self.spinFree.value()) + ',' + '1.0' + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'SET,' + '5,' +  '칼로리버너' + ',' + '칼로리버너' + ',' + str(self.spinCal.value()) + ',' + '1.0' + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))

        sendStr = 'EXTR,' + '1,' + str(self.spinProtein.value()) + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'EXTR,' + '2,' + str(self.spinMg.value()) + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'EXTR,' + '3,' + str(self.spinElect.value()) + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'EXTR,' + '4,' + str(self.spinFree.value()) + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))
        sendStr = 'EXTR,' + '5,' + str(self.spinCal.value()) + '\n'
        self.ser.write(bytes(sendStr, 'utf-8'))

        self.logEdit.append("[SEND] " + sendStr)
        self.logEdit.moveCursor(QTextCursor.End)

    def on_return(self):
        self.textProgress.setPlainText("추천받기 버튼을 누르세요!")
        self.stackedWidget.setCurrentIndex(0)

    def onComPortsComboBoxIndexChanged(self, idx):
        if self.ser != None:
            self.serialReadThread.terminateThead()
            self.ser.close()
            # self.btnRecommend.setEnabled(False)
            self.btnExtract.setEnabled(False)
            self.btnReturn.setEnabled(False)
        if idx != 0:
            self.ser = serial.Serial(self.comPortsComboBox.currentText(), 9600)
            self.serialReadThread.start()
            # self.btnRecommend.setEnabled(True)
            self.btnExtract.setEnabled(True)
            self.btnReturn.setEnabled(True)


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
                    print("[RECV] " + self.readLine)
                    self.readLine = ''
                else:
                    self.readLine += chr(c)

    def terminateThead(self):
        self.exitThread = True
        # 쓰레드 종료
        self.quit()
        self.wait(5000)

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = DrinkRecommendationApp()
    window.show()
    sys.exit(app.exec_())
