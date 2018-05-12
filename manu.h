#ifndef manu_h
#define manu_h
#define max_options_one_screen 5
#include <Adafruit_ssd1306syp.h> 

class Menu;
typedef int (*PF)(Menu*);

extern const unsigned char LOGO[];
class Menu {
    public:
        Menu(Adafruit_ssd1306syp *dp);
        void update();
        void drawLOGO();
        void drawMenu();
        Adafruit_ssd1306syp* getDisplay();
        void addOption(String text, PF pf);
        void operate(int key[]);
        
    private:
        Adafruit_ssd1306syp* display;
        String text_options[16];
        PF pf_options[16];
        int num_options;
        int cursor;
        int cursor_seen;
};
#endif
