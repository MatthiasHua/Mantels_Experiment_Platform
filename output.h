#ifndef output_h
#define output_h
#include <Adafruit_ssd1306syp.h> 

class Out {
  public:
      Out(int O_RCLK, int O_SRCLK, int O_SER);
      void output_byte(unsigned char date);
  private:
      int RCLK;
      int SRCLK;
      int SER;
};

#endif
