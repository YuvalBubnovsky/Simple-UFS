#ifndef MYFILE_H
#define MYFILE_H

#pragma once
#include <stdio.h>
#include <stdarg.h> // https://publications.gbdirect.co.uk//c_book/chapter9/stdarg.html
                    // https://stackoverflow.com/questions/2735587/in-a-c-function-declaration-what-does-as-the-last-parameter-do

struct myFILE
{
    int temp;
    //TODO
};

myFILE *myfopen(const char *restrict pathname, const char *restrict mode);
int myfclose(myFILE *stream);
size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *restrict stream);
size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myFILE *restrict stream);
int myfseek(myFILE *stream, long offset, int whence);
int myfscanf(myFILE *restrict stream, const char *restrict format, ...); // see stdarg.h
int myfprintf(myFILE *restrict stream, const char *restrict format, ...); // see stdarg.h

#endif