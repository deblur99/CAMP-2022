#include "defines.h"
#include "operators.h"
#include "tasks.h"

int main(int argc, char *argv[]) {
    // initialize
    initRegisters();
    char *fullString = (char *)malloc(sizeof(char) * BUF_LENGTH); 
    OPERATOR *expression = (OPERATOR *)malloc(sizeof(OPERATOR));
    FILE *fp = fopen("myinput.txt", "r");
    if (fp == NULL) {
        perror("File Not Found");
        return -1;
    }

    // running actual task
    while (feof(fp) != TRUE) {
        fullString = readFromInputFile(fp, fullString);

        if (!getOperator(expression, fullString)) {
            free(fullString);
            free(expression);
            fclose(fp);
            return -1;
        }

        // TO DO : call executing function with expression struct argument
        // and process it on the called function by value of the argument
        if (!executeExpression(expression)) {
            free(fullString);
            free(expression);
            fclose(fp);
            return -1;
        }

        memset(fullString, '\0', BUF_LENGTH);
    }

    // after all tasks end, terminate program
    free(fullString);
    fclose(fp);

    return 0;
}