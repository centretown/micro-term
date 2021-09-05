// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once
#include <Arduino.h>

class MicroTerm
{
private:
    Stream &serial;
    int led;

    static const size_t in_size = 256;
    char command[in_size] = {0};
    char in[in_size] = {0};
    size_t in_count = 0;
    uint8_t led_state = HIGH;
    bool overflow_detected = false;
    bool prompt_user = true;
    bool command_ready = false;

    void reset();
    void prompt();
    void overflow();
    size_t print(char);
    void blink()
    {
        led_state = (led_state == LOW) ? HIGH : LOW;
        digitalWrite(led, led_state);
    }

public:
    char prefix[128] = {0};

    MicroTerm(Stream &serial, int led) : serial(serial), led(led) {}
    ~MicroTerm() {}

    void setup(const char *p);

    bool ready();
    const char *get();
    size_t println(const char *);
    size_t print(const char *s);
};
