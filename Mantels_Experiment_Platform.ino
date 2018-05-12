#include <ESP8266WiFi.h>
#include <Adafruit_ssd1306syp.h>
#include "manu.h"
#include "keyboard.h"
#include "output.h"
#include "wifi.h"
/*
 * 针脚定义
 */
#define SDA_PIN 13// OLED SDA
#define SCL_PIN 12 // OLED SCL
#define KB_CLK 4
#define KB_SH 5
#define KB_QH 14
#define O_RCLK 3
#define O_SRCLK 1
#define O_SER 9

Adafruit_ssd1306syp *display;
Menu *menu;
Keyboard *keyboard;
Wifi *wifi;
Out *out;
String access_key;
String student_key;

int showlogo(Menu *m);
int testOption2(Menu *m);
int testOption3(Menu *m);
int get_access_key(Menu *m);
int get_student_key_status(Menu *m);
int post_result(Menu *m);
int get_test_case(Menu *m);

void setup()
{
    delay(1000);
    display = new Adafruit_ssd1306syp(SDA_PIN,SCL_PIN);
    display -> initialize();
    menu = new Menu(display);
    keyboard = new Keyboard(KB_CLK, KB_SH, KB_QH);
    out = new Out(O_RCLK, O_SRCLK, O_SER);
    wifi = new Wifi();
    delay(2000);
    menu -> addOption("show LOGO", showlogo);
    menu -> addOption("input test", testOption2);
    menu -> addOption("output test", testOption3);
    menu -> addOption("get accesskey", get_access_key);
    menu -> addOption("get studentkey", get_student_key);
    menu -> addOption("get studentkey status", get_student_key_status);
    menu -> addOption("post result", post_result);
    menu -> addOption("get test case", get_test_case);
}

void loop()
{
    int key[num_key];
    keyboard -> update(key);
    menu -> operate(key);
    menu -> drawMenu();
}

int showlogo(Menu *m) {
    int key[num_key];
    while(1) {
        keyboard -> update(key);
        m -> drawLOGO();
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};

int testOption2(Menu *m) {
    int key[num_key];
    while(1) {
        keyboard -> update(key);
        display -> clear();
        display -> setTextColor(WHITE, BLACK);
        for (int i = 0; i < 8; i++) {
            display -> setCursor(10 * i, 20);
            display -> println(key[8 + i]);
            display -> setCursor(10 * i, 0);
            display -> println(key[16 + i]);
        }
        display -> update();
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};


int testOption3(Menu *m) {
    int key[num_key];
    while(1) {
        int date = 0;
        keyboard -> update(key);
        for (int i = 0; i < 8; i++) {
            date = date * 2 + key[16 + i];
        }
        out -> output_byte(date);
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_access_key(Menu *m) {
    int key[num_key];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting access key ...");
    display -> update();
    String ak = wifi -> get_access_key();
    display -> clear();
    display -> setCursor(0, 0);
    access_key = ak.substring(ak.lastIndexOf('\n') + 1);
    display -> println(access_key);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_student_key(Menu *m) {
    int key[num_key];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting student key ...");
    display -> update();
    String sk = wifi -> get_student_key(access_key);
    display -> clear();
    display -> setCursor(0, 0);
    student_key = sk.substring(sk.lastIndexOf('\n') + 1);
    display -> println(student_key);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_student_key_status(Menu *m) {
    int key[num_key];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting student key status...");
    display -> update();
    String s = wifi -> get_student_key_status(student_key);
    display -> clear();
    display -> setCursor(0, 0);
    student_key = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(student_key);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};

int post_result(Menu *m) {
    int key[num_key];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("posting result...");
    display -> update();
    String s = wifi -> post_result(access_key, student_key);
    display -> clear();
    display -> setCursor(0, 0);
    student_key = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(student_key);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_test_case(Menu *m) {
    int key[num_key];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting test case...");
    display -> update();
    String s = wifi -> get_test_case(access_key, student_key);
    display -> clear();
    display -> setCursor(0, 0);
    student_key = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(student_key);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[5] == 1) {
            return 1;
        }
    }
    return 1;
};
