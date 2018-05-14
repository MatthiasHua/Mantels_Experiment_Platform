#include "keyboard.h"

Keyboard::Keyboard(int kb_CLK, int kb_SH, int kb_QH ) {
    CLK = kb_CLK;
    SH = kb_SH;
    QH = kb_QH;
    pinMode(SH, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(QH, INPUT);
}

unsigned char Keyboard::read_165 (){ 
    int i;
    unsigned char date = 0;
    for(i = 0; i < 8; i++) {
        date = date << 1;
        date = date | digitalRead(QH);
        digitalWrite(CLK, LOW);
        digitalWrite(CLK, HIGH);
    }
    return date;
}

void Keyboard::update() {
    unsigned char date;
    //更新165数据
    digitalWrite(SH,HIGH);
    digitalWrite(SH,LOW);
    digitalWrite(SH,HIGH);
    for (int i = 0; i < num_input; i++){
        if (i % 8 == 0)
          date = read_165();
        key[i] = (date + 1) % 2;
        date = date >>1;
    }
    for (int i = 0; i < 8; i++) {
        if (key[i] == 1 && key_cache[i] == 0)
          key_cache[i] = 1;
        else if (key[i] == 0 && key_cache[i] == 1)
          key_cache[i] = 0;
        else if (key[i] == 1 && key_cache[i] == 1)
          key[i] = 0; 
    }
}

void Keyboard::update(int r_key[]) {
    update();
    for (int i = 0; i < num_input; i++) {
        r_key[i] = key[i];
    }
}

