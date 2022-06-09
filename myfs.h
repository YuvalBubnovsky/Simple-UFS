#ifndef MYFS_H
#define MYFS_H

#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <aio.h> // contains ssize_t and off_t

struct superblock
{
    size_t num_of_inodes;
    size_t num_of_blocks;
    size_t size_of_blocks;
};

struct inode
{
    unsigned int first_block; // address of first block
    size_t size;
    char name[8];
};


struct block
{
    unsigned int next; // address of next block
    char data[512]; //512 was historically used in some version of linux, source: https://www.youtube.com/watch?v=sLR17lUjTpc&ab_channel=C%2FC%2B%2BDublinUserGroup (lecture, not implementation)
};


struct myopenfile
{
    int fd; // file descriptor
    unsigned int permission;
    /*
     * 0 - closed, not supposed to happen! (this struct is for *open* files)
     * 1 - r (read only)
     * 2 - rb (read binary only)
     * 3 - w (read and write)
     * 4 - wb (read and write binary only)
     */
};

struct mydirent 
{
    int d_name[8];
    int fd[20];
};

struct myDIR
{
    struct mydirent ent;
    // TODO
};

// https://man7.org/linux/man-pages/man0/dirent.h.0p.html

void mkfs();
void mysync();
int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data);
int myopen(const char *pathname, int flags);
int myclose(int myfd);
ssize_t myread(int myfd, void *buf, size_t count);
ssize_t mywrite(int myfd, const void *buf, size_t count);
off_t mylseek(int myfd, off_t offset, int whence);
struct myDIR *myopendir(const char *name);
struct mydirent *myreaddir(struct myDIR *dirp);
int myclosedir(struct myDIR *dirp);

#endif