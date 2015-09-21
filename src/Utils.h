#pragma once
#ifndef _UTILS_H
#define _UTILS_H

#include <cstdlib>

//For min/max.
#include <algorithm>

float randomFloat(float min, float max);

float convertScale(float value, float istart, float istop, float ostart, float ostop);
//double convertScale(double value, double istart, double istop, double ostart, double ostop);

float constrain(float value, float istart, float istop);

#endif