#pragma once

#include <App/App.h>

#include <NetworkVariable.h>

#include <Frame/Frame.h>
#include <Frame/Frame_NetworkVariable.h>
#include <Frame/Frame_Time.h>
#include <Frame/Frame_Date.h>

class App_Dashboard : public App
{
public:
    void init() override;
    void select() override;
    void show(int updatesThisSecond) override;

    int getUpdatesPerSecond() { return 1; }

private:
    NetworkVariable _varLeft{"left"};
    NetworkVariable _varRight{"right"};
    
    Frame_NetworkVariable _frameTopLeft {2, 1, 29, 17, &_varLeft};
    Frame_NetworkVariable _frameTopRight {33, 1, 29, 17, &_varRight};
    Frame_Time _frameTime {1, 20, 62, 24};
    Frame_Date _frameDate {2, 46, 60, 17};
};