#include "Frame/Frame_NetworkVariable.h"

void Frame_NetworkVariable::show()
{
    Frame::show();

    const int FONT_SIZE = 1;

    String const& text = _nv->getValue();

    printCentered(text, FONT_SIZE);
}