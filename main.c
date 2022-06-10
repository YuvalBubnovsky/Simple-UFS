#include "myfile.h"
#include "myfs.h"

int main()
{
    mymkfs(1000000);
    mymount("mkfs_0",NULL, NULL, NULL, NULL);
    int fd = myopen("butfirst\\rejoice",1);
    char *str = "hello world";
    int from_write = mywrite(fd, str, 11);
    char from_read[20];
    myread(fd,from_write, 1);
    printf("from mywrite - %d , from myread - %s\n", &from_write);
}