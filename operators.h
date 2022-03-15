#include "defines.h"

char** getOneOperationFromTxt(FILE *fp, char *buf[]); 

void mov(int *op, int val);

int add(int op1, int op2);

int subtract(int op1, int op2);

int multiply(int op1, int op2);

int divide(int op1, int op2);