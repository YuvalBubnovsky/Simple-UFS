#include "myfs.h"

/* GLOBALS */

// note: these two sit on the stack, this is by design.
myDIR dir;
superblock sb;

inode *inodes;
block *blocks;
myopenfile *open_files = NULL;
mydirent *directories;

void mkfs(size_t size)
{
    /* before we begin we check that there is a minimum amount of space for the UFS to function, even if it isn't very practical */
    unsigned int structs_size = sizeof(blocks) + sizeof(inodes) + sizeof(superblock);
    if (size <= structs_size)
    {
        printf("Please allocate at least a minimum of %ls bytes!\n", &structs_size);
        exit(1); // due to the fact that otherwise a segfault may occur in user code.
    }

    /* FIRSTLY, we need a superblock to manage metada, 10% of memory (minus the superblock) to inodes, and the rest goes to blocks */

    unsigned int space_for_inodes = (size - sizeof(superblock)) * 0.1;
    unsigned int space_for_blocks = (size - sizeof(superblock)) * 0.9;

    /* SECONDLY, we allocate memory according to previous calculation */

    sb.num_of_blocks = space_for_blocks / sizeof(blocks); // rounded down
    sb.num_of_inodes = space_for_inodes / sizeof(inodes); // same
    sb.size_of_blocks = sizeof(blocks);                   // this is data_size + 8 = 520 bytes

    /* Prints for Control */
    printf("superblock inodes: %ld\n", sb.num_of_inodes);
    printf("superblock blocks: %ld\n", sb.num_of_blocks);
    printf("superblock blocks size: %ld\n", sb.size_of_blocks);
    /* */

    /* we want to use locals and not globals, because we intend to 'write' the memory to disk and then free the heap space we allocated */
    inode *_inodes;
    block *_blocks;

    /* not using the previous calculations in case they were rounded down! */
    _inodes = malloc(sizeof(inodes) * sb.num_of_inodes);
    _blocks = malloc(sizeof(blocks) * sb.num_of_blocks);
    directories = malloc(num_of_dirents * sizeof(mydirent)); // num_of_dirents = 10 and is plenty for this exercise.

    /*THIRDLY, we initiate all the values to their appropriate control values and etc... */

    unsigned int i, j;
    for (i = 0; i < sb.num_of_inodes; i++) // Initializes inodes
    {
        _inodes[i].first_block = none;
        strcpy(_inodes[i].name, "");
    }
    for (i = 0; i < sb.num_of_blocks; i++) // Initializes blocks
    {
        _blocks[i].next = none;
    }
    for (i = 0; i < num_of_dirents; i++) // Initializes dirents
    {
        for (j = 0; j < fd_size; j++)
        {
            directories[i].fd[j] = -1;
        }
        strcpy(directories[i].d_name, "");
    }

    /* LASTLY, we write all of this into our 'disk', which in this exercise is a regular file instead of one that actually represents hardware. */
    //char filename[7] = strncat("mfks_", (char)(fs_index + '0'), 1);
    //FILE *file = fopen(filename, "w+"); //TODO
    FILE * file = fopen("mkfs","W+");
    fwrite(&sb, sizeof(superblock), 1, file);
    for (i = 0; i < sb.num_of_inodes; i++)
    {
        fwrite(&(_inodes[i]), sizeof(inodes), 1, file);
    }
    for (i = 0; i < sb.num_of_inodes; i++)
    {
        fwrite(&(_blocks[i]), sizeof(blocks), 1, file);
    }
    fclose(file);

    /* we also free the heap space we allocated for the localss, since we wrote this memory to the disk */
    free(_blocks);
    free(_inodes);

    /* */
    dir.ent = directories;
}

int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
    /**
     * @brief
     * in our implementation we ignore evereything except source, we are required to provide this signature
     * but implementation wise we are doing the simplest thing possible.
     * note, we also do the same mount type every time.
     */

    /* We allocate the maximum amount of open files and init their values */
    open_files = malloc(sizeof(myopenfile) * max_files);
    for (int i = 0; i < max_files; i++)
    {
        open_files[i].fd = none;
        open_files[i].permission = none;

        if (access(source, F_OK) == 0)
        {
            FILE *file = fopen(source, "r");
            fread(&sb, sizeof(superblock), 1, file);
            inodes = malloc(sizeof(inodes) * sb.num_of_inodes);
            blocks = malloc(sizeof(blocks) * sb.num_of_blocks);
            fread(inodes, sizeof(inodes), sb.num_of_inodes, file);
            fread(blocks, sizeof(blocks), sb.num_of_blocks, file);
            fclose(file);
        }
        else
        {
            printf("that file system does not exist! exiting...");
            printf("HINT: run mkfs(10000); before this, and make sure you write the filename correctly!");
            exit(1);
        }
    }
    return 0;
}

int is_file_closed(int myfd)
{
    if (open_files[myfd].fd == none)
    {
        return 1;
    }
    return 0;
}

int is_inode_empty(int myfd)
{
    if (inodes[myfd].first_block == none)
    {
        return 1;
    }
    return 0;
}

int find_file(const char *pathname)
{
    /**
     * @brief 
     * helper method that iteraates over inodes and looks for a file.
     */

    for(int i = i; i< sb.num_of_inodes; i++)
    {
        if(!strcmp(inodes[i].name, pathname))
        {
            return i; //found the file, and so return it's index.
        }
    }
    return 0;
}

int find_block()
{
    for (int i = 0; i < sb.num_of_blocks; i++)
    {
        if (blocks[i].next == none)
        {
            return i;
        }
    }

    return -1;
}

int myopen(const char *pathname, int flags)
{
    if (open_files == NULL)
    {
        printf("you must mount a directory first! exiting...");
        return -1;
    }

    const char delimiter[2] = "\\"; // this reads as just '\'
    char str[16];
    char *temp;
    strcpy(str, pathname);
    temp = strtok(str,delimiter);
    int directory = -1;
    if (temp != NULL)
    {
        char *name = myopendir(pathname)->ent->d_name;
        for(int i = 0; i < num_of_dirents; i++)
        {
            if(!strcmp(directories[i].d_name, name))
            {
                directory = i;
                break;
            }
        }
        if (directory == -1)
        {
            return -1;
        }
    }
    int ans, helper, r;
    int index = find_file(pathname);
    if(!index)
    {
        ans = -1;
        int i,j;
        for(i = 0; j < 20; j++)
        {
            if (inodes[i].first_block == none)
            {
                strcpy(inodes[i].name, pathname);
                helper = find_block();
                if (helper == none)
                {
                    return -1;
                }
                inodes[i].first_block = helper;
                for(j = 0; j < fd_size; j++)
                {
                    r= directories[directory].fd[j];
                    if (r == none) // this means we found an empty fd, so we will use it.
                    {
                        directories[directory].fd[j] = i;
                        ans = i;
                        break; // we dont need to continue, since we foundy and empty fd!
                    }
                }
            }
        }
        if (ans == -1) // if we never found an empty fd, the function 'failed'.
        {
            return -1;
        }

        for (int i = 0; i < max_files; i++)
        {
            if(open_files[j].fd == -1)
            {
                open_files[j].fd = ans;
                open_files[j].permission = flags;
                return j;
            }
        }
    }

    return -1;
}

int myclose(int myfd)
{
    open_files[myfd].fd = none;
    open_files[myfd].permission = none;

    return 1;
}

ssize_t myread(int myfd, void *buf, size_t count)
{
    if (is_file_closed(myfd))
    {
        printf("problem with fd!");
        return -1;
    }
    if (is_inode_empty(myfd))
    {
        printf("problem with inodes!");
        return -1;
    }

    char *str = (char *)buf;
    if (count > inodes[myfd].size)
    {
        count = inodes[myfd].size;
        // ensures we dont try to read more than is possible in the next segment (the for loop).
    }

    int b_index = inodes[myfd].first_block;
    int j;
    for (size_t i = 0; i < count; i++)
    {
        if (j == data_size)
        {
            j = 0;
            b_index = blocks[b_index].next; // iterate to next blocks once were done reading this one.
        }
        str[i] = blocks[b_index].next;
        j++;
    }
    str[count] = '\0'; // manually ensures we have a null terminator.
    return count;
}

ssize_t mywrite(int myfd, const void *buf, size_t count)
{
    if (is_file_closed(myfd))
    {
        printf("problem with fd!");
        return -1;
    }
    if (is_inode_empty(myfd))
    {
        printf("problem with inodes!");
        return -1;
    }

    int curr = inodes[myfd].size;
    int b_index = inodes[myfd].first_block;
    while (curr >= data_size)
    {
        /* this segment ensures we write to the latest blocks in the chain, and to the latest position in that blocks. */
        b_index = blocks[b_index].next;
        curr -= data_size;
        /* what do we mean by this:
         * For example say w have size = 1050 bytes, this can be partitioned to 1024+26 bytes
         * 1024 = 512 * 2, which is the size of 2 blocks, so we know to iterate over them twice to reach the third and newest blocks
         * and at that newest blocks we want to write to the 26'th byte.
         */
    }

    int temp;
    for (size_t i = 0; i < count; i++)
    {
        if (curr == data_size)
        {
            temp = find_block();
            if (temp < 0)
            {
                count = i - 1; // this iteration is not added to the count.
                break;         // we update count to be the exact amount of writes we did and then break.
            }
            blocks[b_index].next = temp;
            b_index = temp;
            blocks[b_index].next = last_in_chain; // to indicate this is the latest blocks!
            /* temp is assigned as the current blocks next, and thefore if we need to iterate to the next
               we simply assign the same value to the current blocks index */
            curr = 0;
        }
        blocks[b_index].data[curr] = ((char *)buf)[i];
        curr++;
    }
    inodes[myfd].size += count; // this is the ONLY place where we update an inodes size value.
    return count;
}

off_t mylseek(int myfd, off_t offset, int whence)
{
    //TODO ???
    return -1;
}

struct myDIR *myopendir(const char *name)
{
    /**
     * @brief
     * iterates over all the dirents and searches for the dirent that matches the name var
     * if that does not exist, use the first empty one.
     */
    myDIR* ans = malloc(sizeof(myDIR));
    int empty = -1;
    for (int i = 0; i < num_of_dirents; i++)
    {
        if (!strcmp(directories[i].d_name, name))
        {
            ans->ent = &directories[i];
            return ans;
        }
        else if (!strcmp(directories[i].d_name, "") && (empty < 0)) // find the first empty dirent
        {
            empty = i;
        }
    }

    if (empty != -1)
    {
        strcpy(directories[empty].d_name, name);
        ans->ent = &directories[empty];
    }

    return ans;
}

struct mydirent *myreaddir(struct myDIR *dirp)
{
    /**
     * @brief
     * Simply returns the dirent of the myDIR struct
     */
    mydirent *ans = dirp->ent;
    return ans;
}

int myclosedir(struct myDIR *dirp)
{
    unsigned int b_index, temp, i, j, r;
    for (i = 0; i < num_of_dirents; i++)
    {
        for (j = 0; j < fd_size; j++)
        {
            r = directories[i].fd[j];
            if (r != -1)
            {
                inodes[r].size = 0;
                strcpy(inodes[r].name, "");
                b_index = inodes[r].first_block;
                while (b_index != -2)
                {
                    bzero(blocks[b_index].data, data_size);
                    temp = b_index;
                    b_index = blocks[b_index].next;
                    blocks[temp].next = none;
                }
                inodes[r].first_block = none;
            }
            directories[i].fd[j] = -1;
        }
        strcpy(directories[i].d_name, "");
    }

    return 1;
}

void writebyte(int fd, int opos, char data) { 
    /**
     * @brief Write a SINGLE byte into a disk blocks. 
     * The function calculates the correct relevant blocks (rb) that is needed to be accessed. 
     * if the position that is needed to be wrriten is out of the bounds of the file,
     * allocate a new disk blocks for it. 
     */
    int pos = opos;
    int rb = inodes[fd].first_block;
    while (pos>=data_size) {
        pos-=data_size;
        if (blocks[rb].next==-1) {
            exit(1);
        } else if (blocks[rb].next == -2) { // the current blocks is the last blocks, so we allocate a new blocks
            blocks[rb].next = find_block(); 
            rb = blocks[rb].next;
            blocks[rb].next = -2; 
        } else {
            rb = blocks[rb].next;
        }
    }
    if (opos>inodes[fd].size) {
        inodes[fd].size = opos+1;
    }
    blocks[rb].data[pos] = data;
}

char readbyte(int fd, int pos) {
    /**
     * @brief Read a SINGLE byte from a disk blocks. 
     * The function calculates the correct relevant blocks (rb) that is needed to be accessed. 
     * The single byte is @return 'ed.
     */
    int rb = inodes[fd].first_block;
    while (pos>=data_size) {
        pos-=data_size;
        rb = blocks[rb].next;
        if (rb==-1) {
            return -1;
        } else if (rb == -2) {
            return -1;
        }
    }
    return blocks[rb].data[pos];
}