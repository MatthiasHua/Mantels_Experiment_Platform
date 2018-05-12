#ifndef keyboard_h
#define keyboard_h
#include <Adafruit_ssd1306syp.h> 

#define num_key 24

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
        Keyboard(int KB_CLK, int KB_SH, int KB_QH);
        void update();
        void update(int r_key[]);
        unsigned char read_165();
    private:
        int key[num_key];
        int key_cache[8];
        int CLK;
        int SH;
        int QH;
};
#endif
