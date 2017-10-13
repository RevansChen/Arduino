#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//                lcd_Addr,En,Rw,Rs,d4,d5,d6,d7,backlighPin,pol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 


// 接腳
const byte PIN_POWER = 12;  // 電源開關
const byte PIN_RESET = 9;   // 重置

const byte PIN_CDS = A0;    // 
const byte PIN_PAD_Y = A1;  // 

const byte PIN_LEVEL_SELECT = 2;    // 難度選擇
const byte PIN_LEVEL_EASY = 4;      //     簡單
const byte PIN_LEVEL_MIDDLE = 7;    //     普通
const byte PIN_LEVEL_HARD = 8;      //     困難

const byte PIN_RGBLED_RED = 3;      // 三色LED, 紅色
const byte PIN_RGBLED_GREEN = 5;    // 三色LED, 綠色
const byte PIN_RGBLED_BLUE = 6;     // 三色LED, 藍色

const byte MAX7219_REGADDR_DIGIT_BASEADDR = 0x01;   // 資料暫存器開始位址
const byte MAX7219_REGADDR_DECODE_MODE = 0x09;      // 解碼模式暫存器
const byte MAX7219_REGADDR_INTENSITY = 0x0a;        // 亮度暫存器
const byte MAX7219_REGADDR_SCAN_LIMIT = 0x0b;       // 掃描行數暫存器
const byte MAX7219_REGADDR_SHUTDOWN = 0x0c;         // 關閉模式暫存器
const byte MAX7219_REGADDR_DISPLAY_TEST = 0x0f;     // 測試模式暫存器

const byte MAX7219_SCANLIMIT_8 = 0x07;          // 掃描0~7位
const byte MAX7219_INTESITY_8 = 0x08;           // 中等亮度, duty cycle = 17/32
const byte MAX7219_DECODE_MODE_UNUSED = 0x00;   // 不使用BCD解碼
const byte MAX7219_DISPLAY_TEST_OFF = 0x00;     // 停止 (正常工作下使用這個值)
const byte MAX7219_SHUTDOWN_OFF = 0x01;         // 停止 (正常工作下使用這個值)

const int LEVEL_EASY = 1;
const int LEVEL_MIDDLE = 2;
const int LEVEL_HARD = 3;

const byte xsy[8] = {
    MAX7219_REGADDR_DIGIT_BASEADDR, 
    MAX7219_REGADDR_DIGIT_BASEADDR + 1,
    MAX7219_REGADDR_DIGIT_BASEADDR + 2, 
    MAX7219_REGADDR_DIGIT_BASEADDR + 3, 
    MAX7219_REGADDR_DIGIT_BASEADDR + 4, 
    MAX7219_REGADDR_DIGIT_BASEADDR + 5, 
    MAX7219_REGADDR_DIGIT_BASEADDR + 6, 
    MAX7219_REGADDR_DIGIT_BASEADDR + 7
};

const byte ysy[8] = {
    B00000001,
    B00000010,
    B00000100,
    B00001000,
    B00010000,
    B00100000,
    B01000000,
    B10000000
};

const byte ylevel[6] = {
    B11111000, 
    B11110001, 
    B11100011, 
    B11000111, 
    B10001111, 
    B00011111
};

bool gamest = false;
bool start = false;

int y_data = 0;
int score = 0;

byte g_bright = 0;
byte r_bright = 0;
byte b_bright = 0;

void max7219(const byte reg, const byte data) {
    digitalWrite(SS, LOW);
    SPI.transfer(reg);
    SPI.transfer(data);
    digitalWrite(SS, HIGH);
}

void setup() {
    Serial.begin(9600);
    
    pinMode(PIN_POWER, INPUT);  // 電源開關
    pinMode(PIN_RESET, INPUT);  // 重置
    
    pinMode(PIN_CDS, INPUT);    // cds select
    pinMode(PIN_PAD_Y, INPUT);  // 搖桿Y軸
    
    pinMode(PIN_LEVEL_SELECT, INPUT);   // 難度選擇
    pinMode(PIN_LEVEL_EASY, INPUT);     // 簡單
    pinMode(PIN_LEVEL_MIDDLE, INPUT);   // 普通
    pinMode(PIN_LEVEL_HARD, INPUT);     // 困難
    
    pinMode(PIN_RGBLED_GREEN, OUTPUT);  // 三色LED, 綠色  level- easy
    pinMode(PIN_RGBLED_BLUE, OUTPUT);   // 三色LED, 藍色  level- middle
    pinMode(PIN_RGBLED_RED, OUTPUT);    // 三色LED, 紅色  level- hard
    
    // 初始化 LCD, 一行 16 的字元, 共 2 行, 預設開啟背光
    lcd.begin(16, 2);      
    lcd.backlight();
    lcd.clear();

    // 初始化MAX7219
    SPI.begin();
    max7219(MAX7219_REGADDR_SCAN_LIMIT, MAX7219_SCANLIMIT_8);
    max7219(MAX7219_REGADDR_DECODE_MODE, MAX7219_DECODE_MODE_UNUSED);
    max7219(MAX7219_REGADDR_INTENSITY, MAX7219_INTESITY_8);
    max7219(MAX7219_REGADDR_DISPLAY_TEST, MAX7219_DISPLAY_TEST_OFF);
    max7219(MAX7219_REGADDR_SHUTDOWN, MAX7219_SHUTDOWN_OFF);

    for (byte i = 0; i < 8; i++) {
        max7219(MAX7219_REGADDR_DIGIT_BASEADDR + i, 0);
    }
}

void loop() {
    start = !digitalRead(PIN_POWER);
  
    if (start) {
        lcd.setCursor(0, 0);
        lcd.print("~~~~~Flappy~~~~~");
        lcd.setCursor(0, 1);
        lcd.print("~~~~~~bird~~~~~~");
        selectlevel();
    }
    else {
        lcd.setCursor(0, 0);
        lcd.print("remove card to");
        lcd.setCursor(0, 1);
        lcd.print("start the game");
    }
}

void selectlevel() {
    color();
    
    gamest = 0;
    score = 0;
    
    unsigned int cds = analogRead(PIN_CDS);
    Serial.println(cds);
    
    gameloop(85, 100, cds, LEVEL_EASY);     // level-----easy
    gameloop(100, 120, cds, LEVEL_MIDDLE);  // level-----middle
    gameloop(65, 85, cds, LEVEL_HARD);      // level-----hard
}

void gameloop(int cdsMin, int cdsMax, int cds, int v) {
    String msg1;
    String msg2;
    int delayTime;
    switch (v) {
        case LEVEL_EASY:
            msg1 = "easy";
            msg2 = "******easy******";
            delayTime = 150;
            break;
            
        case LEVEL_MIDDLE:
            msg1 = "middle";
            msg2 = "*****middle*****";
            delayTime = 100;
            break;
            
        case LEVEL_HARD:
            msg1 = "hard";
            msg2 = "******hard******";
            delayTime = 60;
            break;
    }
    
    bool rst;
    while (cdsMin < cds && cds < cdsMax) { 
        Serial.print("you select---");
        Serial.println(msg1);
        Serial.println("yes or no");
        
        lcd.setCursor(0, 0);
        lcd.print(msg2);
        lcd.setCursor(0, 1);
        lcd.print("***yes or no ***");
        
        analogWrite(PIN_RGBLED_GREEN, g_bright);
        analogWrite(PIN_RGBLED_RED, r_bright);
        analogWrite(PIN_RGBLED_BLUE, b_bright);
        r_bright = 0;
        g_bright = 0;
        b_bright = 0;
        
        start = !digitalRead(PIN_POWER);
        bool select = !digitalRead(PIN_LEVEL_SELECT);
        
        // game start
        while (select) {                           
            Serial.println("game start");
            if (!gamest) {
                lcd.setCursor(0, 0);
                lcd.print("***game start***");
                lcd.setCursor(0, 1);
                lcd.print("                ");
            }
            rst = !digitalRead(PIN_RESET);
            game(delayTime);
            
            if (!(rst && start)) break;
        }
        
        rst = !digitalRead(PIN_RESET);
        if (!(rst && start)) break;
    }
}

void color() {
    analogWrite(PIN_RGBLED_GREEN, g_bright);
    analogWrite(PIN_RGBLED_RED, r_bright);
    analogWrite(PIN_RGBLED_BLUE, b_bright);
    
    bool easy = !digitalRead(PIN_LEVEL_EASY);       // G
    if (!easy) {
        r_bright = 0;
        g_bright = 255;
        b_bright = 0;
    }
    
    bool middle = !digitalRead(PIN_LEVEL_MIDDLE);   // B
    if (!middle) {
        r_bright = 0;
        g_bright = 0;
        b_bright = 255;
    }
    
    bool hard = digitalRead(PIN_LEVEL_HARD);        // R
    if (!hard) {
        r_bright = 255;
        g_bright = 0;
        b_bright = 0;
    }
}

void ctrly() {
    int y_val = analogRead(PIN_PAD_Y);
    
    if (y_val < 300) {
        if (y_data > 0) {
            y_data--;
        }
    }
    else if(y_val > 700) {
        if (y_data < 7) {
            y_data++;
        }
    }

    max7219(xsy[0], ysy[y_data]);
    delay(10);
}

void game(int delayTime) {
    randomSeed(analogRead(A5)); 
    int ran = (int)random(0, 6);
    if (!gamest) {
        for (int x = 7; x > 0; x--) {
            ctrly();
            max7219(xsy[x], ylevel[ran]);
            if (x < 7) {
                max7219(xsy[x + 1], 0);
            }
            delay(delayTime);
            
            if (x == 0 && (y_data == ran || y_data == (ran + 1) || y_data == (ran + 2))) {
                score++;
            }
            else if (x == 0 && (y_data != ran || y_data != (ran + 1) || y_data != (ran + 2))) {
                gamest = true;
                break;
            }
        }
    }
    
    if (gamest) {
        Serial.println(score);
        lcd.setCursor(0, 0);
        lcd.print("your bird died..");
        lcd.setCursor(0, 1);
        lcd.print("your score : ");
        lcd.print(score);
    }
}
