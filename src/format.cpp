#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    // more than 100 hours? Could be also modified to 24h
    seconds = seconds % 360000;
    int hours = seconds / 3600;
    int rest = seconds % 3600;
    int minutes = rest / 60;
    seconds = rest % 60;
    return (std::to_string(hours) + ":" + std::to_string(minutes)+ ":" + std::to_string(seconds)); 
}