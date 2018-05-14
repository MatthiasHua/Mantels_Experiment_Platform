#ifndef keyboard_h
#define keyboard_h
#include <Adafruit_ssd1306syp.h> 
#include "config.h"

/*
 * up     0
 * down   1
 * left   2
 * right  3
 * enter  4
 * cancel 5
 */
class Keyboard {
    public:
        Keyboard(int kb_CLK, int kb_SH, int kb_QH);
        void update();
        void update(int r_key[]);
        unsigned char read_165();
    private:
        int key[num_input];
        int key_cache[8];
        int CLK;
        int SH;
        int QH;
};
#endif
