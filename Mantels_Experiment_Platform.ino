#include <ESP8266WiFi.h>
#include <Adafruit_ssd1306syp.h>
#include "manu.h"
#include "keyboard.h"
#include "output.h"
#include "wifi.h"
#include "config.h"
#include "qrcode.h"

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
int get_student_result(Menu *m);
int start_experiment(Menu *m);
int test_qr_code(Menu *m);
int test_qr_code2(Menu *m);

void setup()
{
    delay(1000);
    display = new Adafruit_ssd1306syp(SDA_PIN,SCL_PIN);
    display -> initialize();
    menu = new Menu(display);
    keyboard = new Keyboard(KB_CLK, KB_SH, KB_QH);
    out = new Out(O_RCLK, O_SRCLK, O_SER);
    wifi = new Wifi(DEBUG);
    delay(2000);
    menu -> addOption("show LOGO", showlogo);
    menu -> addOption("start experiment", start_experiment);
    menu -> addOption("input test", testOption2);
    menu -> addOption("output test", testOption3);
    menu -> addOption("get accesskey", get_access_key);
    menu -> addOption("get studentkey", get_student_key);
    menu -> addOption("get studentkey status", get_student_key_status);
    menu -> addOption("post result", post_result);
    menu -> addOption("get test case", get_test_case);
    menu -> addOption("test exp", get_student_result);
    menu -> addOption("QR-CODE", test_qr_code);
    menu -> addOption("QR-CODE2", test_qr_code2);
}

void loop()
{
    int key[num_input];
    keyboard -> update(key);
    menu -> operate(key);
    menu -> drawMenu();
}

int start_experiment(Menu *m) {
    int key[num_input];
    int flag = login();
    if (flag == 0)
        return 0;
    do_experiment();
    return 1;
}

int login() {
    int key[num_input];
    int flag = get_access_key();
    int l_cursor = 0;
    WiFiClient client;
    Serial.begin(9600);
    if (flag == 0)
        return 0;
    flag = get_student_key();
    if (flag == 0)
        return 0;    
    while(flag == 1) {
        keyboard -> update(key);
        drawLoginMenu(l_cursor);
        if (key[KB_Cancel] == 1)
            return 0;
        else if (key[KB_Enter] == 1)
            flag = 0;
        else if (key[KB_Up] == 1)
            l_cursor = 0;
        else if (key[KB_Down] == 1)
            l_cursor = 1;
        delay(40);
    }
    flag = 1;
    while(flag == 1) {
        keyboard -> update(key);
        flag = get_student_key_status(client);
        draw_login_info(l_cursor);
        if (key[KB_Cancel] == 1)
            return 0;
        else if (key[KB_Right] == 1)
            l_cursor = 1;
        else if (key[KB_Left] == 1)
            l_cursor = 0;
        delay(40);
    }
    //display -> clear();
    //display -> setTextColor(WHITE, BLACK);
    //display -> setCursor(0, 0);
    //display -> println("log in success");
    //display -> update();
    //delay(1000);
    return 1;
}

int do_experiment() {
    int key[num_input];
    int em_cursor = 0;
    while(1) {
        int date = 0;
        drawExperimentMenu(em_cursor);
        keyboard -> update(key);
        for (int i = 0; i < Order_Bits; i++)
            date = date * 2 + key[Toggle_Switch_Address + i];
        out -> output_byte(date);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        } else if (key[KB_Enter] == 1 && em_cursor == 1) {
            check_experiment();
        } else if (key[KB_Up] == 1) {
            em_cursor = 0;
        }
        else if (key[KB_Down] == 1) {
            em_cursor = 1;     
        }
    }
    return 1;
}

int get_access_key() {
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("logging ...");
    display -> update();
    String ak = wifi -> get_access_key();
    access_key = ak.substring(ak.lastIndexOf('\n') + 1);
    return 1;
};

int get_student_key() {
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("logging ...");
    display -> update();
    String sk = wifi -> get_student_key(access_key);
    student_key = sk.substring(sk.lastIndexOf('\n') + 1);
    return 1;
};

void draw_login_info(int login_method) {
    if (login_method == 0) {
        display -> clear();
        display -> setCursor(12, 0);
        display -> println("Please login with    WeChat.");
        display -> setCursor(46, 30);
        display -> println(student_key);
        display -> drawRect(40, 25, 48, 18, WHITE);
        display -> update();
    } else {
        show_QR_CODE(3, "http://mantels.top/user_base/wechat/signin/" + student_key);
    }
}

void drawLoginMenu(int cursor) {
    display -> clear();
    display -> setCursor(46, 12);
    if (cursor == 0) {
        display -> fillRect(20, 7, 88, 18, WHITE);
        display -> setTextColor(BLACK, WHITE); 
    } else {
        display -> drawRect(20, 7, 88, 18, WHITE);
        display -> setTextColor(WHITE, BLACK); 
    }
    display -> println("WeChat");
    display -> setCursor(43, 36);
    if (cursor == 1) {
        display -> fillRect(20, 31, 88, 18, WHITE);
        display -> setTextColor(BLACK, WHITE); 
    } else {
        display -> drawRect(20, 31, 88, 18, WHITE);
        display -> setTextColor(WHITE, BLACK); 
    }
    display -> println("QR Code");
    display -> update();
}


void drawExperimentMenu(int cursor) {
    display -> clear();
    display -> setCursor(25, 12);
    if (cursor == 0) {
        display -> fillRect(20, 7, 88, 18, WHITE);
        display -> setTextColor(BLACK, WHITE); 
    } else {
        display -> drawRect(20, 7, 88, 18, WHITE);
        display -> setTextColor(WHITE, BLACK); 
    }
    display -> println("do experiment");
    display -> setCursor(28, 36);
    if (cursor == 1) {
        display -> fillRect(20, 31, 88, 18, WHITE);
        display -> setTextColor(BLACK, WHITE); 
    } else {
        display -> drawRect(20, 31, 88, 18, WHITE);
        display -> setTextColor(WHITE, BLACK); 
    }
    display -> println("check result");
    display -> update();
}

int get_student_key_status(WiFiClient &client) {
    String s = wifi -> get_student_key_status(student_key, client);
    String sk_status = s.substring(s.lastIndexOf('\n') + 1);
    if (sk_status == "enable")
        return 2;
    else
        return 1;
};

void check_experiment() {
    int key[num_input];
    int input_table[32];
    int output_table[32];
    int num = 4;
    String returndata = "";
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting test case...");
    display -> update();
    String s = wifi -> get_test_case(access_key, student_key);
    String test_case = s.substring(s.lastIndexOf('\n') + 1);
    for (int i = 0; i < num * Order_Bits; i++) {
        if (test_case[i] == '0')
            input_table[i] = 0;
        else
            input_table[i] = 1;
    }
    check(input_table, output_table, num);
    for (int i = 0; i < num; i++) {
        returndata += String(input_table[i * 8 + 6]) + "-" + String(input_table[i * 8 + 7]) + "-" +  String(output_table[i * 8 + 6]) + "-" + String(output_table[i * 8 + 7]) + ((i != num - 1) ? "~" : "");
    }
    post_result(returndata);
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return;
        }
    }
    return;
}

void post_result(String data) {
    int key[num_input];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("posting result...");
    display -> update();
    String s = wifi -> post_result(access_key, student_key, data);
    display -> clear();
    display -> setCursor(0, 0);
    String resultdata = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(resultdata);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return;
        }
    }
};

int showlogo(Menu *m) {
    int key[num_input];
    while(1) {
        keyboard -> update(key);
        m -> drawLOGO();
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

int testOption2(Menu *m) {
    int key[num_input];
    while(1) {
        keyboard -> update(key);
        display -> clear();
        display -> setTextColor(WHITE, BLACK);
        for (int i = 0; i < 8; i++) {
            display -> setCursor(10 * i, 20);
            display -> println(key[LED_Address + i]);
            display -> setCursor(10 * i, 0);
            display -> println(key[Toggle_Switch_Address + i]);
        }
        display -> update();
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};


int testOption3(Menu *m) {
    int key[num_input];
    while(1) {
        int date = 0;
        keyboard -> update(key);
        for (int i = 0; i < 8; i++) {
            date = date * 2 + key[Toggle_Switch_Address + i];
        }
        out -> output_byte(date);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_access_key(Menu *m) {
    int key[num_input];
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
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_student_key(Menu *m) {
    int key[num_input];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting student key ...");
    display -> update();
    String sk = wifi -> get_student_key(access_key);
    display -> clear();
    display -> setCursor(0, 0);
    111sk.substring(sk.lastIndexOf('\n') + 1);
    display -> println(student_key);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_student_key_status(Menu *m) {
    int key[num_input];
    WiFiClient client;
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting student key status...");
    display -> update();
    String s = wifi -> get_student_key_status(student_key, client);
    display -> clear();
    display -> setCursor(0, 0);
    String sk_status = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(sk_status);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

int post_result(Menu *m) {
    int key[num_input];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("posting result...");
    display -> update();
    String s = wifi -> post_result(access_key, student_key, "啦啦啦~");
    display -> clear();
    display -> setCursor(0, 0);
    String resultdata = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(resultdata);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

int get_test_case(Menu *m) {
    int key[num_input];
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    display -> setCursor(0, 0);
    display -> println("getting test case...");
    display -> update();
    String s = wifi -> get_test_case(access_key, student_key);
    display -> clear();
    display -> setCursor(0, 0);
    String resultdata = s.substring(s.lastIndexOf('\n') + 1);
    display -> println(resultdata);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
};

void check(int input_table[], int output_table[], int num) {
    int key[num_input]; 
    for (int i = 0; i < num; i++) {
        int data = 0;
        for (int j = 0; j < 8; j++) {
            data = data * 2 + input_table[i * 8 + j];
        }
        out -> output_byte(data);
        delay(5);
        keyboard -> update(key);
        for (int j = 0; j < 8; j++) {
            output_table[i * 8 + j] = (key[LED_Address + j] + 1) % 2;
        }
        delay(5);
    }
    
}

int get_student_result(Menu *m) {
    int key[num_input];
    int input_table[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1};
    int output_table[32];
    int num = 4;
    check(input_table, output_table, num);
    display -> clear();
    display -> setTextColor(WHITE, BLACK);
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < 8; j++) {
            display -> setCursor(j * 10, i * 10);
            display -> println(output_table[i * 8 + j]);
        }
    }
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
}

int test_qr_code(Menu *m) {
    int key[num_input];
    Serial.begin(9600);
    Serial.println("======");
    String data = wifi -> get("/api/get/qr_code/iot_login/n5FNqS");
    Serial.println("======");
    data = data.substring(data.lastIndexOf('\n') + 1);
    display -> clear();
    display -> fillRect(0, 0, 128, 64, WHITE);
    Serial.println("data");
    Serial.println(data);
    for (int i = 0; i < 29; i++)
        for (int j = 0; j < 29; j++)
          if (data[i * 29 + j] == '1')
            display -> fillRect(2 * j, 2 * i, 2, 2, BLACK);
    display -> update();
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
}

int test_qr_code2(Menu *m) {
    int key[num_input];
    QRCode qrcode;
    uint8_t qrcodeBytes[qrcode_getBufferSize(3)];
    show_QR_CODE(3, "http://mantels.top/user_base/wechat/signin/" + student_key);
    while(1) {
        keyboard -> update(key);
        delay(40);
        if (key[KB_Cancel] == 1) {
            return 1;
        }
    }
    return 1;
}

void show_QR_CODE(int version, String data) {
    QRCode qrcode;
    char c_data[100];
    for (int i = 0; i < data.length(); i++)
      c_data[i] = data[i];
    c_data[data.length()] = '\0';
    uint8_t qrcodeBytes[qrcode_getBufferSize(version)];
    qrcode_initText(&qrcode, qrcodeBytes, version, ECC_LOW, c_data);
    display -> clear();
    display -> fillRect(0, 0, 128, 64, WHITE);
    for (uint8 y = 0; y < qrcode.size; y++) {
        for (uint8 x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y))
                display -> fillRect(35 + 2 * x, 3 + 2 * y, 2, 2, BLACK);
        }
    }
    display -> update();
}

