#pragma once

#ifndef _TASKS_H
#define _TASKS_H
#endif

#include "operators.h"

// for execution
char* readFromInputFile(FILE *fp, char *buffer);

int getOperator(OPERATOR *expression, char *raw);

int executeExpression(OPERATOR *expression);

int showCalcResult(OPERATOR *expression, int result);