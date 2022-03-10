#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

char* getInputFromTxt(FILE *fp, char *buf) {
    char *tok = (char *)malloc(sizeof(char) * 200);
    
    memset(buf, '\0', sizeof(buf));

    while (feof(fp) != TRUE) {
        fscanf(fp, "%s", tok);
        strcat(buf, tok);
        memset(tok, '\0', sizeof(tok));
    }

    free(tok);

    return buf;
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen("./myinput.txt", "r");
    char *buf = (char *)malloc(sizeof(char) * 200);
    char *procBuf = (char *)malloc(sizeof(char) * 200);

    strcpy(procBuf, getInputFromTxt(fp, buf));
    
    printf("%s\n", procBuf);    
    printf("%c %c %c\n", procBuf[0], procBuf[1], procBuf[2]);    

    free(procBuf);
    free(buf);

    fclose(fp);

    return 0;
}