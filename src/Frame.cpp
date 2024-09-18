#include "Frame.h"

constexpr Frame::Size Frame::FONT_SIZES[];

void Frame::show()
{
    g_display.drawRect(_pos.x, _pos.y, _size.w, _size.h, g_colorSecondary);
}

Frame::Size Frame::margins(Size content) const
{
    return {
        (_size.w - content.w) / 2,
        (_size.h - content.h) / 2
    };
}

Frame::Size Frame::margins(int charCount, int fontSize) const
{
    return margins({charCount * FONT_SIZES[fontSize].w, FONT_SIZES[fontSize].h});
}

void Frame::printCentered(String const &text, int fontSize) const
{
    const Size MARGINS = margins(text.length(), fontSize);
    g_display.setTextColor(g_colorPrimary);
    g_display.setTextSize(fontSize);
    g_display.setCursor(_pos.x + MARGINS.x + 1,
                      _pos.y + MARGINS.y);
    g_display.print(text);
}