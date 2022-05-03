#include <stdio.h>

typedef struct _Bar {
    int a;
    float b;
    double c;
}Bar;

void set(int foo[]) {
    foo[0] += 10;
    foo[1] += 20;
    foo[2] += 30;
}

void set2(Bar bar) {
    bar.a += 10;
    bar.b += 20.5;
    bar.c += 30.33;
}

void set3(Bar *bar) {
    bar->a += 10;
    bar->b += 20.5;
    bar->c += 30.33;
}

int main() {
    int foo[3] = {1, 2, 3};

    printf("Before call : %d %d %d\n", *foo, *(foo+1), *(foo+2));
    set(foo);
    printf("After call : %d %d %d\n", *foo, *(foo+1), *(foo+2));

    Bar bar = {10, 12.4f, 15.7};

    printf("Before call : %d %f %lf\n", bar.a, bar.b, bar.c);
    set2(bar);
    printf("Before call : %d %f %lf\n", bar.a, bar.b, bar.c);

    printf("Before call : %d %f %lf\n", bar.a, bar.b, bar.c);
    set3(&bar);
    printf("Before call : %d %f %lf\n", bar.a, bar.b, bar.c);
    
    return 0;
}