#include "output.h"

Out::Out(int o_RCLK, int o_SRCLK, int o_SER) {
    RCLK = o_RCLK;
    SRCLK = o_SRCLK;
    SER = o_SER;
    pinMode(RCLK, OUTPUT);
    pinMode(SRCLK, OUTPUT);
    pinMode(SER, OUTPUT);
    output_byte(0);
}

void Out::output_byte(unsigned char date) {
    for (int i = 0; i < 8; i++) {
        if (date % 2 == 1)
            digitalWrite(SER, HIGH);
        else 
            digitalWrite(SER, LOW);
        digitalWrite(SRCLK, HIGH);
        digitalWrite(SRCLK, LOW);
        digitalWrite(SRCLK, HIGH);
        date = date >> 1;      
    }
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    digitalWrite(RCLK, HIGH);
}

