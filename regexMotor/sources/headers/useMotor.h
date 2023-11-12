#pragma once

#include "createMotor.h"
#include "coloredPrints.h"

void plotMotor(reMotor* motor);
void displayState(State* state);
void nextStep(reMotor* motor, char inputChar);
