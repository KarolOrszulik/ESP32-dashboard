#pragma once

#include <Arduino.h>
#include "Frame.h"
#include "NetworkVariable.h"

class Frame_NetworkVariable : public Frame
{
public:
    Frame_NetworkVariable(Size pos, Size size, NetworkVariable *nv)
        : Frame(pos, size), _nv(nv)
    {
    }

    void show() override
    {
        Frame::show();

        const int FONT_SIZE = 1;

        String text = _nv->_value;

        printCentered(text, FONT_SIZE);
    }

private:
    NetworkVariable *_nv;
};