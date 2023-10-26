/**
 * Develop a multi threaded producer consumer application. (15 points)
        1. Three thread application, wherein each thread invokes at fixed (for 1st thread 10sec, 2nd thread 20sec and 3rd thread 30sec)
        2. When thread will invoke, it should read next chunk of 1KB from the target file. 
            After reading chunk apply unique encryption scheme (e.g. add 0x50 on every byte of 1kb chunk). Each thread having unique encryption scheme.
            After encryption, dumps encrypted chunk to another file.
        3. At a time simultaneously only two threads can read from a target file. If at a time two threads are ready to read, 
            then both threads should be able to read data from target file.

 *
 *
 * Solution for 1: Use sleep(seconds) function
 * Solution for 2: Will be using memory mapped IPC
 * Solution for 3: Will be using counting semaphores and intialize with the count of 2

*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <semaphore.h>

#define NO_OF_THREADS   3
#define TEN_SEC         10
#define FILE_SIZE       1024
#define ENCRP_KEY       0x50


const char* read_file_path = "/tmp/tmpfile";
const char* write_file_path = "/tmp/tmpfile1";

/* A semaphore counting the number of jobs in the queue. */
sem_t read_count;

long read_file_size;

typedef struct thread_params
{
    int id;             /* Thread ID*/
    int invoke_s;       /* Thread invocation time */
    void* file_mem_r;   /* Memory map for reading */
    void* file_mem_w;   /* Memory map for write */
    int offset  ;       /* Offset to include in memory map */


}thread_p;

void encrypt(char* p, size_t len)
{
    for(size_t i = 0; i < len; i++)
    {
        //Just add 0x50 
        p[i] = p[i] + ENCRP_KEY;
    }
}

void* create_read_map()
{
    int fd;
    /* Open the file. */
    fd = open(read_file_path, O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    // Get the file size
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) 
    {
        perror("fstat");
        close(fd);
        exit(1);
    }
    printf("File Size: %ld\n",file_stat.st_size);

    //Get File Size 
    read_file_size = file_stat.st_size;

    /* Create the memory mapping. */
    char* file_memory = mmap(NULL, file_stat.st_size, PROT_READ , MAP_PRIVATE, fd, 0);  //Cannot use Offset as memory is mapped in the multiples of 4K
    if (file_memory == MAP_FAILED) 
    {
        perror("mmap");
        close(fd);
        exit(1);
    }
    close(fd); //No need for file descriptor now

    return file_memory;

}

void* create_write_map()
{
    int fd;
    /* Open the file for writing. */
    fd = open(write_file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) 
    {
        perror("open");
        exit(1);
    }

    // Extend the file size to accommodate the data if necessary
    if (lseek(fd, read_file_size, SEEK_SET) == -1) 
    {
        perror("lseek");
        close(fd);
        exit(1);
    }
    if (write(fd, "", 1) == -1) 
    {
        perror("write");
        close(fd);
        exit(1);
    }

    /* Create the memory mapping. */
    char* file_memory = mmap(NULL, read_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) 
    {
        perror("mmap");
        close(fd);
        exit(1);
    }

    return file_memory;

}

void read_file(char* r_map, char* buffer, size_t size)
{
    //Semaphore for 2 file reading
    sem_wait(&read_count);  //For Blocking semaphore wait
  
    /* Read the bytes */
    char* c = r_map;
    memcpy(buffer,c,size);
        
    printf("Read File before Encryption: \n");
    //printf("%s\n",buffer);
    fwrite(buffer,1,size,stdout);
    fflush(stdout);

    sem_post(&read_count);

}
void write_file(char* w_map, char* buffer, size_t size)
{
    /* Write the bytes */
    char *c = w_map;
    memcpy(c,buffer,size);

    printf("Write File: \n");
    //printf("%s\n",buffer);
    fwrite(buffer,1,size,stdout);
    fflush(stdout);

}

void* thread_function(void* arg)
{
    char buff[2048];
    thread_p* p = (thread_p*) arg;

    sleep(p->invoke_s);
    printf("Thread %d invoked after %d seconds\n",p->id, p->invoke_s);


    //Read the file
    read_file(p->file_mem_r,buff, FILE_SIZE);

    /* Apply Encryption */
    encrypt(buff,FILE_SIZE);

    //printf("Read File After Encryption: \n");
    //printf("%s\n",buff);

    /* Write to new file */
    write_file(p->file_mem_w, buff,FILE_SIZE);
    
    
    return NULL;
}

int main()
{

    /* Create a mapped memory for the threads to work - read file and write file */
    char* read_map = create_read_map();
    char* write_map = create_write_map();

    /* Initialize Semphore */
    sem_init(&read_count, 0, 2);

    pthread_t threads_id[NO_OF_THREADS];
    thread_p thread_arr[NO_OF_THREADS];

    for(int i = 0; i < NO_OF_THREADS; i++)
    {
        thread_arr[i].id = i;
        thread_arr[i].file_mem_r = &read_map[thread_arr[i].id*FILE_SIZE];
        thread_arr[i].file_mem_w = &write_map[thread_arr[i].id*FILE_SIZE];
        switch (i+1)
        {
        case 1:
            thread_arr[i].invoke_s = TEN_SEC;
            break;

        case 2:
            thread_arr[i].invoke_s = TEN_SEC * 2;
            break;
        case 3:
            thread_arr[i].invoke_s = TEN_SEC * 3;
            break;
        default:
            break;
        }
        pthread_create(&(threads_id[i]), NULL, thread_function, (void*)&thread_arr[i]);
    }

    //Wait for threads to join
    for(int i = 0; i < NO_OF_THREADS; i++)
    {
        pthread_join(threads_id[i], NULL);
    }

    /* Release the memory (unnecessary because the program exits). */
    if( munmap(read_map, read_file_size) == -1)
    {
        perror("munmap");
    }
    if (munmap(write_map, read_file_size) == -1) 
    {
        perror("munmap");
    }

    return 0;
}

