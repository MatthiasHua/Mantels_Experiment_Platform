#include "json.h"

void JSON::reset() {
    data = (String)"{";
}

void JSON::add(String key, String s) {
    data += (String)"\"" + key + "\":\"" + s + "\",";
}

void JSON::add(String key, int num) {
    data += (String)"\"" + key + "\":" + String(num) + ",";
}


String JSON::get() {
    return data.substring(0, data.length() - 1) + (String)"}";
}

