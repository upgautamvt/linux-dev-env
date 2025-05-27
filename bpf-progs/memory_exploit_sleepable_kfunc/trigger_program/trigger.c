//
// Created by upgautam on 5/27/25.
//

#include <unistd.h>
#include <fcntl.h>
int main() {
    int fd = creat("/tmp/hello.txt", 0644);
    close(fd);
    unlink("/tmp/hello.txt");
    return 0;
}
