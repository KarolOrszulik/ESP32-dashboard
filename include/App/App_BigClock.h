#pragma once

#include "App/App.h"

class App_BigClock : public App
{
public:
    void init() override;
    void select() override;
    void show(int updatesThisSecond) override;

    int getUpdatesPerSecond() { return 3; }

private:
    static constexpr int NUM_POINTS = 180;

    struct Point {int x; int y;};

    Point _points[NUM_POINTS] = {
        {0, -32}, {1, -32}, {2, -32}, {3, -32}, {4, -32}, {5, -32},
        {6, -31}, {7, -31}, {8, -31}, {9, -31},
        {10, -30}, {11, -30},
        {12, -29}, {13, -29}, {14, -29},
        {15, -28},
        {16, -27}, {17, -27},
        {18, -26},
        {19, -25}, {20, -25}, 
        {21, -24},
        {22, -23}
    };
};