#pragma once

#include <Arduino.h>
#include "Frame/Frame.h"

class Frame_Time : public Frame
{
public:
    using Frame::Frame;

    void show() override;
};