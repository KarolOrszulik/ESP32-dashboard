#pragma once

class App
{
public:
    virtual void init() = 0;
    virtual void select() = 0;
    virtual void show(int updatesThisSecond = 0) = 0;

    virtual int getUpdatesPerSecond() = 0;
};