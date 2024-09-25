#pragma once

#include <Arduino.h>
#include "Frame/Frame.h"

class Frame_Date : public Frame
{
public:
    using Frame::Frame;

    void show() override;
};