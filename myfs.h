#ifndef MYFS_H
#define MYFS_H

#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <aio.h> // contains ssize_t and off_t
#include <unistd.h>
#include <stdlib.h>

#define max_files 10000
#define data_size 512
#define path_size 8
#define fd_size 20
/* some consts for radability! */
const int none = -1;
const int last_in_chain = -2;
const unsigned int num_of_dirents = 10;
/* workaround for creating several ufs's */
int fs_index = 0;

struct superblock
{
    size_t num_of_inodes;
    size_t num_of_blocks;
    size_t size_of_blocks;
};

struct inode
{
    int first_block; // index of first block
    size_t size;
    char name[path_size];
};


struct block
{
    int next; // index of next block
    char data[data_size]; //512 was historically used in some version of linux, source: https://www.youtube.com/watch?v=sLR17lUjTpc&ab_channel=C%2FC%2B%2BDublinUserGroup (lecture, not implementation)
};


struct myopenfile
{
    int fd; // file descriptor
    unsigned int permission;
};

struct mydirent 
{
    char d_name[path_size];
    int fd[fd_size];
};

struct myDIR
{
    struct mydirent* ent;
};

typedef struct superblock superblock;
typedef struct inode inode;
typedef struct block block;
typedef struct myopenfile myopenfile;
typedef struct mydirent mydirent;
typedef struct myDIR myDIR;

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
void write_single(int fd, int opos, char data);
char read_single(int fd, int pos);

#endif