#include "myfile.h"
#include "myfs.h"

int main()
{
    mkfs(100000);  
    mymount("mkfs",NULL, NULL, 0, NULL);
    int fd = myopen("mkfs",1);
    char *str = "helloworld";
    ssize_t from_write = mywrite(fd, str, 10);
    printf("bytes from mywrite: %ld\n", from_write);
    char *buffer = malloc(sizeof(char) * 11);
    for(int i = 0; i < 10; i++)
    {
        buffer[i] = '_';
    }
    buffer[10] = '\0';
    ssize_t from_read = myread(fd, buffer, 10);
    printf("bytes from myread: %ld\n", from_read);
    printf("string from myread: %s\n", buffer);
    myclose(fd);
    //free(buffer); // oddly enoguh, causes a segfault. (buffer is a main local and does not get re-alloced\re-malloced anywhere...)
}