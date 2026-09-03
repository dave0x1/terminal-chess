#ifndef UTILITY_H
#define UTILITY_H

#include "moveArray.h"

typedef enum {
    RESULT_ERROR = -1,
    RESULT_OK = 0,
}Result;

void printArray(MoveArray* arr);

#endif