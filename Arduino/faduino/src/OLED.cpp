#include "OLED.h"
#include "ASCFont.h"
#include "KSFont.h"

Adafruit_SSD1306 display(OLED_RESET);

byte HANFontImage[32] = {
    0,
};
byte HANFontImageExpand[128] = {
    0,
};

// 16x16 폰트 16x32로 확대하는 함수(한글)
void expandImage16x16To32x32(const byte *inputImage, byte *outputImage)
{
    // inputImage: 16x16 크기의 원본 이미지 (32바이트)
    // outputImage: 32x32 크기의 확대된 이미지 (128바이트)
    memset(outputImage, 0, 128); // 초기화

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            // inputImage에서 원본 픽셀 가져오기
            int inputIndex = (y * 16 + x) / 8; // 바이트 인덱스
            int inputBit = 7 - (x % 8);        // 비트 위치
            bool pixel = (inputImage[inputIndex] >> inputBit) & 1;

            // outputImage에서 2x2 블록 설정
            int outputX = x * 2;
            int outputY = y * 2;
            for (int dy = 0; dy < 2; ++dy)
            {
                for (int dx = 0; dx < 2; ++dx)
                {
                    int outX = outputX + dx;
                    int outY = outputY + dy;
                    int outputIndex = (outY * 32 + outX) / 8; // 바이트 인덱스
                    int outputBit = 7 - (outX % 8);           // 비트 위치
                    if (pixel)
                    {
                        outputImage[outputIndex] |= (1 << outputBit);
                    }
                }
            }
        }
    }
}

// 8x16 폰트 16x32로 확대하는 함수(영문,숫자)
void expandImage8x16To16x32(const byte *inputImage, byte *outputImage)
{
    // inputImage: 8x16 크기의 원본 이미지 (16바이트)
    // outputImage: 16x32 크기의 확대된 이미지 (64바이트)
    memset(outputImage, 0, 64); // 출력 버퍼 초기화

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            // inputImage에서 원본 픽셀 가져오기
            int inputIndex = y;   // inputImage는 각 행이 1바이트로 저장
            int inputBit = 7 - x; // 비트 위치
            bool pixel = (inputImage[inputIndex] >> inputBit) & 1;

            // outputImage에서 2x2 블록 설정
            int outputX = x * 2;
            int outputY = y * 2;
            for (int dy = 0; dy < 2; ++dy)
            {
                for (int dx = 0; dx < 2; ++dx)
                {
                    int outX = outputX + dx;
                    int outY = outputY + dy;
                    int outputIndex = (outY * 16 + outX) / 8; // 바이트 인덱스
                    int outputBit = 7 - (outX % 8);           // 비트 위치
                    if (pixel)
                    {
                        outputImage[outputIndex] |= (1 << outputBit);
                    }
                }
            }
        }
    }
}

void TCA9548A(uint8_t bus)
{
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    Wire.endTransmission();
}

void initDisplays()
{
    Wire.begin();
    Wire.setWireTimeout(100000, true); // I2C Timeout Set, 100ms
    for (int i = 0; i < 5; i++)
    {
        TCA9548A(i);
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        // display.clearDisplay();
        drawDisplay(i, "", "", 0);
        // display.display();
    }
}

void drawDisplay(unsigned int oledIdx, const char *userName, const char *drugName, unsigned int dose)
{
    TCA9548A(oledIdx);
    display.clearDisplay();

    matrixPrint(0, 0, userName);
    matrixPrint(0, 32, String(dose).c_str());
    matrixPrint(72, 32, "mL");

    display.display();
}

void drawWait(unsigned int oledIdx)
{
    TCA9548A(oledIdx);
    matrixPrint(0, 43, "대기>");
    display.display();
}

void drawExtr(unsigned int oledIdx)
{
    TCA9548A(oledIdx);
    matrixPrint(43, 43, "추출>");
    display.display();
}

void drawDone(unsigned int oledIdx)
{
    TCA9548A(oledIdx);
    matrixPrint(85, 43, "완료!");
    display.display();
}

void clearDisplay(unsigned int oledIdx)
{
    TCA9548A(oledIdx);
    display.clearDisplay();
}

void matrixPrint(int XPOS, int YPOS, const char *pChar)
{
    byte *pFs;
    byte c, c2, c3;

    while (*pChar)
    {
        c = *(byte *)pChar++;

        //---------- 한글 ---------
        if (c >= 0x80)
        {
            c2 = *(byte *)pChar++;
            c3 = *(byte *)pChar++;
            pFs = getHAN_font(c, c2, c3);
            expandImage16x16To32x32(pFs, HANFontImageExpand);
            display.drawBitmap(XPOS, YPOS, HANFontImageExpand, 32, 32, 1);
            XPOS = XPOS + 32;
            if (XPOS > 128)
            {
                XPOS = 0;
                YPOS = YPOS + 32;
            }
        }
        //---------- ASCII ---------
        else
        {
            pFs = ASCfontSet + ((byte)c - 0x20) * 16;
            expandImage8x16To16x32(pFs, HANFontImageExpand);
            display.drawBitmap(XPOS, YPOS, HANFontImageExpand, 16, 32, 1);
            XPOS = XPOS + 24;
            if (XPOS > 128)
            {
                XPOS = 0;
                YPOS = YPOS + 32;
            }
        }
    }
}

/*=============================================================================
      한글 font 처리부분
  =============================================================================*/

byte *getHAN_font(byte HAN1, byte HAN2, byte HAN3)
{

    const byte cho[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0};
    const byte cho2[] = {0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5};
    const byte jong[] = {0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1};

    uint16_t utf16;
    byte first, mid, last;
    byte firstType, midType, lastType;
    byte i;
    byte *pB, *pF;

    /*------------------------------
      UTF-8 을 UTF-16으로 변환한다.

      UTF-8 1110xxxx 10xxxxxx 10xxxxxx
    */
    utf16 = (HAN1 & 0x0f) << 12 | (HAN2 & 0x3f) << 6 | (HAN3 & 0x3f);

    /*------------------------------
      초,중,종성 코드를 분리해 낸다.

      unicode = {[(초성 * 21) + 중성] * 28}+ 종성 + 0xAC00

            0   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
      초성 ㄱ   ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
      중성 ㅏ   ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ
      종성 없음 ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
    ------------------------------*/
    utf16 -= 0xac00;
    last = utf16 % 28;
    utf16 /= 28;
    mid = utf16 % 21;
    first = utf16 / 21;

    first++;
    mid++;

    /*------------------------------
      초,중,종성 해당 폰트 타입(벌)을 결정한다.
    ------------------------------*/

    /*
     초성 19자:ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ
     중성 21자:ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ
     종성 27자:ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅆㅇㅈㅊㅋㅌㅍㅎ

     초성
        초성 1벌 : 받침없는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
        초성 2벌 : 받침없는 'ㅗㅛㅡ'
        초성 3벌 : 받침없는 'ㅜㅠ'
        초성 4벌 : 받침없는 'ㅘㅙㅚㅢ'
        초성 5벌 : 받침없는 'ㅝㅞㅟ'
        초성 6벌 : 받침있는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
        초성 7벌 : 받침있는 'ㅗㅛㅜㅠㅡ'
        초성 8벌 : 받침있는 'ㅘㅙㅚㅢㅝㅞㅟ'

     중성
        중성 1벌 : 받침없는 'ㄱㅋ' 와 결합
        중성 2벌 : 받침없는 'ㄱㅋ' 이외의 자음
        중성 3벌 : 받침있는 'ㄱㅋ' 와 결합
        중성 4벌 : 받침있는 'ㄱㅋ' 이외의 자음

     종성
        종성 1벌 : 중성 'ㅏㅑㅘ' 와 결합
        종성 2벌 : 중성 'ㅓㅕㅚㅝㅟㅢㅣ'
        종성 3벌 : 중성 'ㅐㅒㅔㅖㅙㅞ'
        종성 4벌 : 중성 'ㅗㅛㅜㅠㅡ'

    */
    if (!last)
    { // 받침 없는 경우
        firstType = cho[mid];
        if (first == 1 || first == 24)
            midType = 0;
        else
            midType = 1;
    }
    else
    { // 받침 있는 경우
        firstType = cho2[mid];
        if (first == 1 || first == 24)
            midType = 2;
        else
            midType = 3;
        lastType = jong[mid];
    }
    memset(HANFontImage, 0, 32);

    // 초성
    pB = HANFontImage;
    pF = KSFont + (firstType * 20 + first) * 32;
    i = 32;
    while (i--)
        *pB++ = pgm_read_byte(pF++);

    // 중성
    pB = HANFontImage;
    pF = KSFont + (8 * 20 + midType * 22 + mid) * 32;
    i = 32;
    while (i--)
        *pB++ |= pgm_read_byte(pF++);

    // 종성
    if (last)
    {
        pB = HANFontImage;
        pF = KSFont + (8 * 20 + 4 * 22 + lastType * 28 + last) * 32;
        i = 32;
        while (i--)
            *pB++ |= pgm_read_byte(pF++);
    }

    return HANFontImage;
}

/*=============================================================================

  =============================================================================*/
