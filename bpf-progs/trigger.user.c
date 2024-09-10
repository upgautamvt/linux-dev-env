#include <stdio.h>
#include <unistd.h>

void main() {
    getchar();
    char buf[256];
    getcwd(buf, 256);
}
