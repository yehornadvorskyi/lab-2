#include "predicates.h"

#include <cmath>

using namespace std;

bool isNegative(int x)
{
    return x < 0;
}

bool trigAboveTwo(int x)
{
    double y = sin(x) + cos(x);
    y = sin(y) + cos(y);
    return y > 2.0;
}
