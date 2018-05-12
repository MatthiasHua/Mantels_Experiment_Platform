#ifndef wifi_h
#define wifi_h
#include <ESP8266WiFi.h>
#include <Adafruit_ssd1306syp.h> 
#include "json.h"


class Wifi {
  public:
      Wifi();
      String get_access_key();
      String get_student_key(String ak);
      String get_student_key_status(String sk);
      String get_test_case(String ak, String sk);
      String post_result(String ak, String sk);
      String get(String url);
      String post(String data, String url);
  private:
      char *ssid;
      char *password;
      char *host = "mantels.top";
      int port = 80;
      //char *host = "192.168.1.102";
      //int port = 5000;
};

#endif
