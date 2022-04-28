#include <stdio.h>
#include <stdlib.h>

typedef struct _Foo {
    int a;
    int b;
    int c;
}Foo;

typedef struct _Bar {
    Foo *foo;
}Bar;

Bar* allocate() {
    Bar *mem = (Bar *)malloc(sizeof(Bar));
    return mem;
}

Foo* allocate2() {
    Foo *mem = (Foo *)malloc(sizeof(Foo));
    return mem;
}

void freeMem(Bar *mem) {
    free(mem);
}

void freeMem2(Foo *mem) {
    free(mem);
}

Foo* handle(Bar *bar) {
    bar->foo->a = 10;
    bar->foo->b = 20;
    bar->foo->c = 30;

    return bar->foo;
}

int main() {
    Bar *bar = allocate();
    bar->foo = allocate2();

    bar->foo = handle(bar);

    printf("%d %d %d\n", bar->foo->a, bar->foo->b, bar->foo->c);

    freeMem2(bar->foo);
    freeMem(bar);

    return 0;
}