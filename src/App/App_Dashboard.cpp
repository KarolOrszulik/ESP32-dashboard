#include "App/App_Dashboard.h"

void App_Dashboard::init()
{
    _varLeft.init();
    _varRight.init();
}

void App_Dashboard::select()
{
    // nothing special to do
}

void App_Dashboard::show(int)
{
    _frameTopLeft.show();
    _frameTopRight.show();
    _frameTime.show();
    _frameDate.show();
}