#pragma once

#define PxMATRIX_DOUBLE_BUFFER true
#include <PxMatrix.h>

extern uint16_t g_ledDuration;
extern uint16_t g_colorPrimary;
extern uint16_t g_colorSecondary;

extern PxMATRIX g_display;

// void IRAM_ATTR display_updater();
// void Display_createRefreshTask();
void Display_init();
