#include "myfs.h"

struct superblock sb;
struct inode* inodes;
struct block* blocks;

void mkfs(){
    sb.num_of_inodes = 100;
    sb.num_of_blocks = 1000;
    sb.size_of_blocks = sizeof(struct block);

    // init inodes
    inodes = malloc(sb.num_of_inodes * sizeof(struct inode));
    for(int i=0; i<sb.num_of_inodes; i++){
        inodes[i].first_block = 0;
        inodes[i].size = 0;
        inodes[i].name[0] = '\0';
    }

    // init blocks
    blocks = malloc(sb.num_of_blocks * sizeof(struct block));
    for(int i=0; i<sb.num_of_blocks; i++){
        blocks[i].next = 0;
        blocks[i].data[0] = '\0';
    }


}

void mysync(){
    FILE *file;
    file = fopen("myfs_data", "w+");

    // write superblock
    fwrite(&sb, sizeof(struct superblock), 1, file);

    // write inodes
    for (int i=0; i<sb.num_of_inodes; i++){
        fwrite(&inodes[i], sizeof(struct inode), 1, file);
    }

    // write blocks
    for (int i=0; i<sb.num_of_blocks; i++){
        fwrite(&blocks[i], sizeof(struct block), 1, file);
    }
    fclose(file);

}

int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data){
    FILE *file;
    // TODO: take source argument and create the file-system (and file) in the target path
    file = fopen("myfs_data", "r");

    // read superblock
    fread(&sb, sizeof(struct superblock), 1, file);

    //read inodes
    inodes = malloc(sb.num_of_inodes * sizeof(struct inode));
    fread(inodes, sizeof (struct inode), sb.num_of_inodes, file);
    

   /* for(int i=0; i<sb.num_of_inodes; i++){
        fread(&inodes[i], sizeof(struct inode), 1, file);
    }*/

    //read blocks
    blocks = malloc(sb.num_of_blocks * sizeof(struct block));
    fread(blocks, sizeof (struct block), sb.num_of_blocks, file);

    /*for(int i=0; i<sb.num_of_blocks; i++){
        fread(&blocks[i], sizeof(struct block), 1, file);
    }*/
    
    fclose(file);
}
int myopen(const char *pathname, int flags){

}
int myclose(int myfd){

}
ssize_t myread(int myfd, void *buf, size_t count){

}
ssize_t mywrite(int myfd, const void *buf, size_t count){

}
off_t mylseek(int myfd, off_t offset, int whence){

}
struct myDIR *myopendir(const char *name){

}
struct mydirent *myreaddir(struct myDIR *dirp){

}
int myclosedir(struct myDIR *dirp){

}