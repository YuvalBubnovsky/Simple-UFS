#include "myfile.h"
#include "myfs.h"
#include <math.h>
#include <string.h>

struct inode *inodes; // declaration for compiler

myFILE *myfopen(const char *pathname, const char *mode)
{

    int fd = myopen(pathname, 0);
    if (fd == -1)
    {
        exit(1);
    }

    myFILE *file = (myFILE *)malloc(sizeof(myFILE));
    strcpy(file->mode, mode);
    file->fd = fd;
    file->ptr = 0;
    switchs(mode)
    {
        cases("a")
            file->ptr = inodes[fd].size;

        cases("w") 
            for (size_t i = 0; i < file->size; i++)
            {
                file->data[i] = ' ';
            }
            file->data[file->size] = '\0';

        cases("r")
            for (size_t i = 0; i < file->size; i++)
            {
            file->data[i] = read_single(fd, i);
            }

        defaults
            file->ptr = 0;
    } switchs_end;
    myclose(fd);
    return file;
}
int myfclose(myFILE *stream)
{
    
    for (size_t i = 0; i < stream->size; i++)
    {
        write_single(stream->fd, i, stream->data[i]);
    }

    free(stream->data);
    free(stream);
    return 1;
}
size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    size_t i;
    int elements = size * nmemb; // number of elements to read
    char *buffer = malloc(elements + 1);
    int original_ptr = stream->ptr;

    while(stream->ptr + i > stream->size){
        buffer[i] = stream->data[stream->ptr + i];
        original_ptr++;
        i++;
    }

    stream->ptr = original_ptr;
    buffer[elements] = '\0';
    strncpy(ptr, buffer, elements);
    free(buffer);

    return stream->ptr;
}
size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    if(stream->mode == "w" || stream->mode == "a" || stream->mode == "r" || stream->mode == "r+"){
        void *tmp = ptr;
        int i;
        for (i = 0; i < nmemb; i++)
        {
          //  stream->data[stream->ptr + i] = tmp[i];
            myfwrite(stream->ptr,tmp,tmp,i);
            tmp++;
        }
        return nmemb;
    }
    else {
        printf("Error: file is not in write mode\n");
        return 0;
    }
}
int myfseek(myFILE *stream, long offset, int whence)
{

    switch(whence){
        case SEEK_SET:
            stream->ptr = offset;
            break;

        case SEEK_CUR:
            stream->ptr += offset;
            break;
        
        case SEEK_END:
            stream->ptr = stream->size + offset;
            break;

        default:
            stream->ptr=0;
    }
    return stream->ptr;
}