// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include <MicroTerm.h>

void MicroTerm::setup(const char *prefix)
{
    // keep prefix short
    int len = strlen(prefix);
    if (len < 3)
    {
        prefix = "unk";
    }
    const char *pre = prefix + len - 3;
    strncpy(prompt_text, pre, sizeof(prompt_text));
    strncat(prompt_text, "> ", sizeof(prompt_text));
    stream.flush();
}

void MicroTerm::reset()
{
    in_count = 0;
    memset(in, 0, in_size);
}

void MicroTerm::prompt()
{
    if (prompt_user)
    {
        println("");
        print(prompt_text);
        prompt_user = false;
    }
}

void MicroTerm::overflow()
{
    if (overflow_detected)
    {
        reset();
        if (println("overflow"))
        {
            overflow_detected = false;
        }
    }
}

template <class T>
size_t MicroTerm::prt(T s, bool newline)
{
    size_t res = 0;
    if (stream.availableForWrite())
    {
        res = 1;
        stream.print(s);
        if (newline) {
            stream.println();
        }
        stream.flush();
    }
    return res;
}

size_t MicroTerm::print(char c)
{
    return prt(c, false);
}

size_t MicroTerm::print(const __FlashStringHelper *s)
{
    return prt(s, false);
}

size_t MicroTerm::print(const char *s)
{
    return prt(s, false);
}

size_t MicroTerm::println(const __FlashStringHelper *s)
{
    return prt(s, true);
}

size_t MicroTerm::println(const char *s)
{
    return prt(s, true);
}

bool MicroTerm::ready()
{
    size_t echo_count = in_count;
    int c = -1;
    while (stream.available())
    {
        c = stream.read();
        if (c == -1)
        {
            continue;
        }

        // discard cr
        if (c == 13)
        {
            continue;
        }

        // acknowledge lf
        if (c == 10)
        {
            prompt_user = true;
            if (in_count == 0)
            {
                continue;
            }
            if (!command_ready)
            {
                in[in_count] = 0;
                strncpy(command, in, sizeof(command));
                command_ready = true;
            }
            reset();
            continue;
        }

        in[in_count] = c;
        if (++in_count >= in_size)
        {
            reset();
            overflow_detected = true;
        }
    }

    if (overflow_detected)
    {
        overflow();
    }
    else if (user_on)
    {
        if (echo_count < in_count)
        {
            print(in + echo_count);
        }
        else if (prompt_user)
        {
            prompt();
        }
    }

    return command_ready;
}

const char *MicroTerm::get()
{
    if (command_ready)
    {
        command_ready = false;
        return command;
    }
    return NULL;
}
