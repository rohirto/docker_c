/**
 * For each exercise select whether you will use connection-oriented or connectionless socket with reason for your choice. 
 * By default server should serve multiple-clients at a time
 * 
 * 
 * Develop a simple file-transfer client server application with following functionalities (20 points):
 *      1. Server on-startup should scan pre-defined directory and create list of files.
 *      2. As soon as server can accept client connections it should on demand present the file list to client.
 *      3. Server on demand should transfer the selected file to client as soon as possible.
 *      4. If demanded, Server should be able to provide file information (metadata) like size, type, creation date to client.
 *      5. Server should have idle timeout of 120 seconds, i.e. if any client has no activity till specified timeout, server should terminate the connection with that client.
 *      6. Client on start-up or as demanded by user should fetch file list from server.
 *      7. As per file selected by user, client should ask server to transfer that file.
 *      8. If demanded by user, client should fetch information of file from server.
 * 
 * Connection Oriented Approach needed here: 
 * 
 * Can use threads for concurrency on server side
 * Go step wise
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

const char* dir_path = "../ipc/";

struct FileInfo 
{
    char name[256];
    off_t size;
    time_t creation_time;
};

void print_files(int file_count, struct FileInfo* file_list) 
{
    printf("List of files in the directory:\n");
    for (int i = 0; i < file_count; i++) 
    {
        printf("Name: %s, Size: %ld bytes, Creation Time: %s", file_list[i].name, file_list[i].size, ctime(&file_list[i].creation_time));
    }
    printf("Total files: %d\n", file_count);
}

/**
 * 1. Server on-startup should scan pre-defined directory and create list of files.
*/
int list_all_files(struct FileInfo** file_list, int* file_count) 
{
    DIR* dp = opendir(dir_path);
    if (dp == NULL) 
    {
        perror("opendir");
        return -1; // Indicate error
    }

    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL) 
    {
        if (entry->d_type == DT_REG) {
            struct FileInfo* temp = realloc(*file_list, (*file_count + 1) * sizeof(struct FileInfo));
            if (temp == NULL) 
            {
                perror("realloc");
                return -1; // Indicate error
            }
            *file_list = temp;

            strcpy((*file_list)[*file_count].name, entry->d_name);

            struct stat st;
            char path[256];
            sprintf(path, "%s%s", dir_path, entry->d_name);
            if (stat(path, &st) == 0) 
            {
                (*file_list)[*file_count].size = st.st_size;
            } 
            else 
            {
                (*file_list)[*file_count].size = -1;
            }

            (*file_list)[*file_count].creation_time = st.st_ctime;

            (*file_count)++;
        }
    }

    closedir(dp);

    return 0; // Indicate success
}

int main() 
{
    struct FileInfo* file_list = NULL;
    int file_count = 0;

    //Do the part one of Exercise
    if (list_all_files(&file_list, &file_count) != 0) 
    {
        perror("files initialize");
        exit(1);
    }

    // See if the file list is created or not
    print_files(file_count, file_list);

    free(file_list);

    return 0;
}

