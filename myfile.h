#ifndef MYFILE_H
#define MYFILE_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> // https://publications.gbdirect.co.uk//c_book/chapter9/stdarg.html
                    // https://stackoverflow.com/questions/2735587/in-a-c-function-declaration-what-does-as-the-last-parameter-do
#include "switchs.h"

typedef struct myFILE {
	char* data;
	char mode[3];
	int fd;
	int size;
	int ptr;
}myFILE;


myFILE* myfopen(const char *pathname, const char *mode);
int myfclose(myFILE *stream);
size_t myfread(void * ptr, size_t size, size_t nmemb, myFILE * stream);
size_t myfwrite(const void * ptr, size_t size, size_t nmemb, myFILE * stream);
int myfseek(myFILE *stream, long offset, int whence);

#endif