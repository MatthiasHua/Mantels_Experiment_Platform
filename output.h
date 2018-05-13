#ifndef output_h
#define output_h
#include <Adafruit_ssd1306syp.h> 

class Out {
  public:
      Out(int o_RCLK, int o_SRCLK, int o_SER);
      void output_byte(unsigned char date);
  private:
      int RCLK;
      int SRCLK;
      int SER;
};

#endif
