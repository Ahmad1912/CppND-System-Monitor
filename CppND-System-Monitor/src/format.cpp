#include <string>

#include "format.h"

using std::string;
using namespace std;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int h, m, s;
    h = seconds / 3600;
    m = (seconds % 3600) / 60;
    s = (seconds % 3600) % 60;

    return to_string(h) + ":" + to_string(m) + ":" + to_string(s); }