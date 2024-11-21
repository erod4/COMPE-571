#include "global.h"

float calculate_energy(int power, int time)
{
    // power in mW, time in seconds
    return power * 0.0001 * time;
}