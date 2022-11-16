/**
 * Factorial template class
 */
#include "TFactorial.h"

int Factorial(int i)
{
    return (i == 0) || (i == 1)? 1: i * Factorial(i - 1);
}