#include "wifi.h"

Wifi::Wifi() {
    Serial.begin(9600);
    //ssid = "MERCURY_304";
    //password = "@sls304@";
    ssid = "Vento";
    password = "0123456789";
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println(WiFi.localIP());
}

String Wifi::get_access_key() {
    JSON json;
    json.reset();
    json.add("number", 123);
    json.add("token", "j9JX9yxTHSACue7t");
    json.add("device name", "test-device-1");
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

String Wifi::get(String url) {
    Serial.print("connecting to ");  
    Serial.println(host);  
    
    WiFiClient client;  
    
    if (!client.connect(host, port)) {  
        Serial.println("connection failed");  
        return "";
    }  
    Serial.println("connection success");  

    String getRequest =(String)("GET ") + url + " HTTP/1.1\r\n" +  
          "Host: " + host + ":" + port + "\r\n" +              
          "Connection: close\r\n\r\n";  
     Serial.println(getRequest);  
     client.print(getRequest);  
     while (client.connected()) {
        if (client.available()) {
            String data = client.readString();
            Serial.println(data);
            client.stop();
            Serial.println("end of client");
            return data;
        }
     }
     delay(100); 
}

String Wifi::post(String data, String url) {
    Serial.print("connecting to ");  
    Serial.println(host);  
    
    WiFiClient client;  
    
    if (!client.connect(host, port)) {  
        Serial.println("connection failed");  
        return "";
    }  
    Serial.println("connection success");  
    int length = data.length(); 
    String postRequest =(String)("POST ") + url + " HTTP/1.1\r\n" +  
          "Content-Type: application/json;charset=utf-8\r\n" +  
          "Host: " + host + ":" + port + "\r\n" +            
          "Content-Length: " + length + "\r\n" +  
          "Connection: Keep Alive\r\n\r\n" +  
          data+"\r\n";  
     Serial.println(postRequest);  
     client.print(postRequest);  
     while (client.connected()) {
        if (client.available()) {
            String returndata = client.readString();
            Serial.println(returndata);
            client.stop();
            Serial.println("end of client");
            return returndata;
        }
     }
     delay(100); 
}
