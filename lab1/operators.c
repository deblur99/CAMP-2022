#include "operators.h"

int mov(OPERATOR *expression) {
    if (*expression->op1 != 'R') {
        return -1;
    }

    *(registers + expression->op1[1]) = strtol(expression->op2, NULL, 16);

    expression->preValue = *(registers + expression->op1[1]);

    return (int)strtol(expression->op2, NULL, 16);
}

int add(OPERATOR *expression) {
    // 1) Rn + Rm -> Rn
    // 2) Rn + number -> Rn
    // 3) number + number -> R0

    // 1) case
    if (expression->op1[0] == 'R' && expression->op2[0] == 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) += *(registers + expression->op2[1]);
        return *(registers + expression->op1[1]);
    }

    // 2) case
    if (expression->op1[0] == 'R' && expression->op2[0] != 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) += strtol(expression->op2, NULL, 16);
        return *(registers + expression->op1[1]);
    } else if (expression->op1[0] != 'R' && expression->op2[0] == 'R') {
        *(registers + expression->op2[1]) += strtol(expression->op1, NULL, 16);
        return *(registers + expression->op2[1]);
    }

    // 3) case
    if (expression->op1[0] != 'R' && expression->op2[0] != 'R') {
        *registers = strtol(expression->op1, NULL, 16) + strtol(expression->op2, NULL, 16);
        return *registers;
    }

    return 0;
}

int subtract(OPERATOR *expression) {
    // 1) Rn - Rm -> Rn
    // 2) Rn - number -> Rn
    // 3) number - number -> R0

    // 1) case
    if (expression->op1[0] == 'R' && expression->op2[0] == 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) -= *(registers + expression->op2[1]);
        return *(registers + expression->op1[1]);
    }

    // 2) case
    if (expression->op1[0] == 'R' && expression->op2[0] != 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) -= strtol(expression->op2, NULL, 16);
        return *(registers + expression->op1[1]);
    } else if (expression->op1[0] != 'R' && expression->op2[0] == 'R') {
        *registers = strtol(expression->op1, NULL, 16) - *(registers + expression->op2[1]);
        return *registers;
    }

    // 3) case
    if (expression->op1[0] != 'R' && expression->op2[0] != 'R') {
        *registers = strtol(expression->op1, NULL, 16) - strtol(expression->op2, NULL, 16);
        return *registers;
    }

    return 0;
}

int multiply(OPERATOR *expression) {
    // 1) Rn * Rm -> Rn
    // 2) Rn * number -> Rn
    // 3) number * number -> R0

    // 1) case
    if (expression->op1[0] == 'R' && expression->op2[0] == 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) *= *(registers + expression->op2[1]);
        return *(registers + expression->op1[1]);
    }

    // 2) case
    if (expression->op1[0] == 'R' && expression->op2[0] != 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) *= strtol(expression->op2, NULL, 16);
        return *(registers + expression->op1[1]);
    } else if (expression->op1[0] != 'R' && expression->op2[0] == 'R') {
        *registers = strtol(expression->op1, NULL, 16) * (*(registers + expression->op2[1]));
        return *registers;
    }

    // 3) case
    if (expression->op1[0] != 'R' && expression->op2[0] != 'R') {
        *registers = strtol(expression->op1, NULL, 16) * strtol(expression->op2, NULL, 16);
        return *registers;
    }

    return 0;
}

int divide(OPERATOR *expression) {
    // 1) Rn / Rm -> Rn
    // 2) Rn / number -> Rn
    // 3) number / number -> R0

    // 1) case
    if (expression->op1[0] == 'R' && expression->op2[0] == 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) /= *(registers + expression->op2[1]);
        return *(registers + expression->op1[1]);
    }

    // 2) case
    if (expression->op1[0] == 'R' && expression->op2[0] != 'R') {
        expression->preValue = *(registers + expression->op1[1]);
        *(registers + expression->op1[1]) /= strtol(expression->op2, NULL, 16);
        return *(registers + expression->op1[1]);
    } else if (expression->op1[0] != 'R' && expression->op2[0] == 'R') {
        *registers = strtol(expression->op1, NULL, 16) / (*(registers + expression->op2[1]));
        return *registers;
    }

    // 3) case
    if (expression->op1[0] != 'R' && expression->op2[0] != 'R') {
        *registers = strtol(expression->op1, NULL, 16) / strtol(expression->op2, NULL, 16);
        return *registers;
    }

    return 0;
}

int compare(OPERATOR *expression) {
    // R or 0x
    // op1 >= op2 -> R0 = 0
    // op1 < op2 -> R0 = 1

    if (expression->op1[0] == 'R' && expression->op2[0] == 'R') {
        if (*(registers + expression->op1[1]) >= *(registers + expression->op2[1])) {
            return 0;
        } else {
            return 1;
        }        
    }

    // 2) case
    if (expression->op1[0] == 'R' && expression->op2[0] != 'R') {
        if (*(registers + expression->op1[1]) >= strtol(expression->op1, NULL, 16)) {
            return 0;
        } else {
            return 1;
        }
    }

    // 3) case
    if (expression->op1[0] != 'R' && expression->op2[0] != 'R') {
        if (strtol(expression->op1, NULL, 16) >= strtol(expression->op2, NULL, 16)) {
            return 0;
        } else {
            return 1;
        }
    }

    return -1;
}

int halt() {
    printf("********* Program terminated.                        *********\n");
    return FALSE;
}