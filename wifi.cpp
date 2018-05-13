#include "wifi.h"
#include "config.h"
Wifi::Wifi(int debug_mode) {
    debug = debug_mode;
    if (debug == 1)
        Serial.begin(9600);
    ssid = dSSID;
    password = dPASSWORD;
    log_s(ssid);
    log_s(password);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        log_s(".");
    }
    log_s("");
    log_s(WiFi.localIP());
}

String Wifi::get_test_case(String ak, String sk) {
    JSON json;
    json.reset();
    json.add("access key", ak);
    json.add("student key", sk);
    json.add("device name", "test-device-1");
    String data = json.get();
    String returndata = post(data, "/api/get/current_experiment/test_case");
    return returndata;
}

String Wifi::post_result(String ak, String sk, String post_data) {
    JSON json;
    json.reset();
    json.add("access key", ak);
    json.add("student key", sk);
    json.add("device name", Device_Name);
    json.add("content", post_data);
    String data = json.get();
    String returndata = post(data, "/api/experiment/new_result_iot");
    return returndata;
}

String Wifi::get_access_key() {
    JSON json;
    json.reset();
    json.add("number", Number);
    json.add("token", Token);
    json.add("device name", Device_Name);
    json.add("time", 7200);
    String data = json.get();
    String returndata = post(data, "/api/access_key_iot");
    return returndata;
}

String Wifi::get_student_key(String ak) {
    JSON json;
    json.reset();
    json.add("access key", ak);
    json.add("last time", 7200);
    String data = json.get();
    String returndata = post(data, "/api/student_key_iot");
    return returndata;
}

String Wifi::get_student_key_status(String sk) {
    String data = get("/api/get/student_key_status/" + sk);
    return data;
}

void Wifi::log_s(String s) {
    if (debug == 1)
        Serial.print(s);
}

void Wifi::log_s(IPAddress s) {
    if (debug == 1)
        Serial.print(s);
}

String Wifi::get(String url) {
    log_s("connecting to ");  
    log_s(host);  
    
    WiFiClient client;  
    
    if (!client.connect(host, port)) {  
        log_s("connection failed");  
        return "";
    }  
    log_s("connection success");  

    String getRequest =(String)("GET ") + url + " HTTP/1.1\r\n" +  
          "Host: " + host + ":" + port + "\r\n" +              
          "Connection: close\r\n\r\n";  
     log_s(getRequest);  
     client.print(getRequest);  
     while (client.connected()) {
        if (client.available()) {
            String data = client.readString();
            log_s(data);
            client.stop();
            log_s("end of client");
            return data;
        }
     }
     delay(100); 
}

String Wifi::post(String data, String url) {
    log_s("connecting to ");  
    log_s(host);  
    
    WiFiClient client;  
    
    if (!client.connect(host, port)) {  
        log_s("connection failed");  
        return "";
    }  
    log_s("connection success");  
    int length = data.length(); 
    String postRequest =(String)("POST ") + url + " HTTP/1.1\r\n" +  
          "Content-Type: application/json;charset=utf-8\r\n" +  
          "Host: " + host + ":" + port + "\r\n" +            
          "Content-Length: " + length + "\r\n" +  
          "Connection: Keep Alive\r\n\r\n" +  
          data+"\r\n";  
     log_s(postRequest);  
     client.print(postRequest);  
     while (client.connected()) {
        if (client.available()) {
            String returndata = client.readString();
            log_s(returndata);
            client.stop();
            log_s("end of client");
            return returndata;
        }
     }
     delay(100); 
}
