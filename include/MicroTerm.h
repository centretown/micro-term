// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once
#include <Arduino.h>

class MicroTerm
{
private:
    Stream &serial;
    char *in;
    size_t in_size;

    char prompt_text[8] = {0};
    char command[81] = {0};
    size_t in_count = 0;
    uint8_t led_state = HIGH;
    bool overflow_detected = false;
    bool prompt_user = true;
    bool command_ready = false;
    bool user_on = true;

    void reset();
    void prompt();
    void overflow();
    size_t print(char);

public:
    MicroTerm(Stream &serial,
              char *in,
              size_t in_size) : serial(serial),
                                in(in),
                                in_size(in_size)
    {
        memset(in, 0, in_size);
    }
    ~MicroTerm() {}

    void setup(const char *p);

    bool ready();
    const char *get();
    size_t println(const char *);
    size_t print(const char *s);
    void user(bool on) { user_on = on; }
};
