/*
    Este módulo define a automatização da irrigação
*/

#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

void initTimer();
bool isIrrigationDue();

#endif
