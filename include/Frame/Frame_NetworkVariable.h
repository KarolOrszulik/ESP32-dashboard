#pragma once

#include <Arduino.h>

#include "Frame/Frame.h"
#include "NetworkVariable.h"

class Frame_NetworkVariable : public Frame
{
public:
    Frame_NetworkVariable(Size pos, Size size, NetworkVariable *nv)
        : Frame(pos, size), _nv(nv)
    {
    }

    Frame_NetworkVariable(int posx, int posy, int sizex, int sizey, NetworkVariable *nv)
        : Frame(posx, posy, sizex, sizey), _nv(nv)
    {
    }

    void show() override;

private:
    NetworkVariable *_nv;
};