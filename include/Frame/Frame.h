#pragma once

#include <Arduino.h>
#include <PxMatrix.h>

extern uint16_t g_colorPrimary;
extern uint16_t g_colorSecondary;
extern struct tm g_timeInfo;
extern PxMATRIX g_display;

class Frame
{
public:
    struct Size
    {
        union
        {
            int x, w;
        };
        union
        {
            int y, h;
        };
    };

    static constexpr Size FONT_SIZES[] = {{0, 0}, {6, 7}, {12, 14}, {18, 24}};

    Frame(Size pos, Size size) : _pos(pos), _size(size) {};
    Frame(int posx, int posy, int sizex, int sizey) : _pos({posx, posy}), _size({sizex, sizey}) {};
    
    virtual void show() = 0;
    Size margins(Size content) const;
    Size margins(int charCount, int fontSize) const;

    void printCentered(String const &text, int fontSize) const;

protected:
    Size _pos, _size;
};
