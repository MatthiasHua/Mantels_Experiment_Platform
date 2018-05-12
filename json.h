#ifndef json_h
#define json_h
#include "Arduino.h"

class JSON {
    public:
        void reset();
        void add(String key, String s);
        void add(String key, int num);
        String get();
    private:
        String data;
};

#endif
