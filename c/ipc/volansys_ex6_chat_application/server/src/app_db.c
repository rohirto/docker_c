/**
 * @file app_db.c
 * @author rohit S
 * @brief Database Code to store server data
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 *  * Username Database format
 * [UserId][comma 1 byte][Username - 8 bytes][LF - 1 byte]
 * ..
 * ..
 * <EOF>
 * 
 * 
 * Passwsord Database format 
 * [UserId][comma][Password][LF - 1 Byte]
 * ..
 * ..
 * <EOF>
 * 
 * Status Database Format
 * [UserId][comma][1/0][LF -1 Byte]
 * ..
 * ..
 * <EOF>
 * 
 * Steps to Access the DB Files
 * step 1: fopen_db_files
 * step 2: Perform db operations
 * step 3: fclose_db_files
 */

#include <stdio.h>
#include <string.h>
#include "app_db.h"
#include "app_socket.h"
#include "app_debug.h"

//GLobal Variables
const char* username_file = "../db/username.txt";
const char* password_file = "../db/passwd.txt";
const char* status_file = "../db/status.txt";

/**
 * @brief Open the file descriptors of data bases, needs to be complemeted with close function always
 * 
 * @return int 0 on success -1 on failure
 * 

 */
int fopen_db_files()
{
    server_cnxt* init_server_context = get_server_context();
    if(init_server_context != NULL)
    {
        //Add the File pointers
        if((init_server_context->username = fopen(username_file,"a+")) != NULL &&
            (init_server_context->password =fopen(password_file,"a+")) != NULL &&
            (init_server_context->status = fopen(status_file,"r+")) != NULL
        )
        {
            //File pointers are Ok
            //init_server_context->no_of_active_connections = 0;
            //debugLog2("All file pointers OK\n");

            return 0;
        }
    }
    debugError("fopen db files");
    return -1;
}

/**
 * @brief Close the file descriptors of data bases, needs to be complemeted with open function always
 * 
 * @return int 0 on success -1 on failure
 */
int fclose_db_files()
{
    server_cnxt *free_Server_cnxt = get_server_context();
    if (free_Server_cnxt != NULL)
    {
        if (fclose(free_Server_cnxt->username) == 0 &&
            fclose(free_Server_cnxt->password) == 0 &&
            fclose(free_Server_cnxt->status) == 0)
        {
            // Files closed
            //debugLog2("All file pointers closed successfully\n");
            return 0;
        }
    }
    debugError("fclose db files");
    return -1;
}

/**
 * @brief Search a username in db, if not found then add it, max 128 users allowed
 * 
 * @param username to be seached or entered
 * @return int userID on success -1 on failure
 */
int fsearch_db(char *username)
{
    server_cnxt *search_Server_cnxt = get_server_context();
    if (search_Server_cnxt != NULL)
    {
        char line[256];
        int userId = -1, i =0;
        int lastuserID = -1;

        // Read the file line by line
        while (fgets(line, sizeof(line), search_Server_cnxt->username) != NULL)
        {
            char storedUsername[9]; // Assuming usernames are 8 characters long
            int storedUserId;

            // Parse the line to extract UserId and Username
            if (sscanf(line, "%d,%8[^,\n]", &storedUserId, storedUsername) == 2)
            {
                // Compare the given username with the stored one
                if (strcmp((char*)username, storedUsername) == 0)
                {
                    userId = storedUserId;
                    debugLog2("Found existing user: %d\n", userId);
                    if(onlinestatus(userId) == 1)
                    {
                        //User Already online!
                        return -2;
                    }
                    break;
                }
                lastuserID = storedUserId;
            }
            if(i++ > MAX_USERS)
            {
                fprintfRed(stdout,"MAX USERS REACHED\n");
                break;
            }
        }
        // If UserId is not found, create a new entry
        if (userId == -1)
        {
            fseek(search_Server_cnxt->username, 0, SEEK_END); // Move to the end of the file

            // Get the UserId for the new entry
            int newUserId = lastuserID + 1;

            // Append the new entry to the file
            fprintf(search_Server_cnxt->username, "%d,%s\n", newUserId, username);
            userId = newUserId;
            debugLog2("Added new userID: %d\n", userId);
        }

        return userId;
        
    }
    debugError("fsearch");
    return -1;
}

/**
 * @brief Handler for username database
 * 
 * @param username to be seached
 * @return int userID on success -1 on failure
 */
int username_handling(char* username)
{
    int retval;
    //Open the file descriptors
    if(fopen_db_files() == -1)
    {
        debugError("file open");
        return -1;
    }

    if((retval = fsearch_db(username)) == -1)
    {
        debugError("file search");
        //No need to return from here as files needs to be closed
    }

    if(fclose_db_files() == -1)
    {
        debugError("file close");
        return -1;

    }

    return retval;

}

/**
 * @brief Update status in db, if not found then add it, max 128 users allowed
 * 
 * @param userID of whos status needs to be updated
 * @param status Online or Offline
 * @return int 0 on success -1 on failure
 */
int update_status_db(int userID, int status)
{
    server_cnxt *status_Server_cnxt = get_server_context();
    if (status_Server_cnxt != NULL)
    {
        // Read the file line by line
        int storedUserId, storedStatus;
        long currentPosition;
        int found = 0;
        int i = 0;

        // Loop through the file to find the userID
        while (fscanf(status_Server_cnxt->status, "%d,%d", &storedUserId, &storedStatus) == 2)
        {
            if (storedUserId == userID)
            {
                // Update the online status for the found userID
                found = 1;
                currentPosition = ftell(status_Server_cnxt->status);
                fseek(status_Server_cnxt->status, currentPosition - strlen("1"), SEEK_SET); // Move back to the start of the status
                fprintf(status_Server_cnxt->status, "%d", status); // Update the status
                debugLog2("userId: %d Status updated\n", userID);
                break; // No need to continue searching
            }
            i++;
        }
        if(!found)
        {
            if(i > MAX_USERS)
            {
                fprintfRed(stdout,"MAX USERS REACHED\n");
                return -1;
            }
            debugLog2("userId: %d Not found\n", userID);
            // If userID is not found, add a new entry at the end of the file
            fseek(status_Server_cnxt->status, 0, SEEK_END);
            fprintf(status_Server_cnxt->status, "%d,%d\n", userID, status);
            debugLog2("userId: %d Status updated\n", userID);
        }

        return 0;
    }
    debugError("update status");
    return -1;
}

/**
 * @brief Handler function for status DB
 * 
 * @param userID of whos status needs to be updated
 * @param status Online or Offline
 * @return int int 0 on success -1 on failure
 */
int status_handling(int userID, int status)
{
    int retval = 0;
    if(userID < 0)
    {
        return -2;  //Errors clients have this userID
    }
    //Open the file descriptors
    if(fopen_db_files() == -1)
    {
        debugError("file open");
        return -1;
    }

    //Update the status in database
    if(update_status_db(userID,status) == -1)
    {
        debugError("update status");
        retval = -1;
        
    }

    if(fclose_db_files() == -1)
    {
        debugError("file close");
        return -1;

    }

    return retval;
    

}

/**
 * @brief Send the list of Online User to a newly joined client
 * 
 * @param socket of the newly joined client
 * @param selfUserID The userID of newly joined client, need not be sent to him
 * @return int 0 on success, -1 on failure
 */
int sendOnlineUsernames(int socket, int selfUserID)
{
    
    server_cnxt *sendnames_Server_cnxt = get_server_context();
    if (sendnames_Server_cnxt != NULL)
    {
        int userID, onlineStatus, found = 0;
        char username[9]; // Adjust the size as needed

        // Loop through both files simultaneously
        while (fscanf(sendnames_Server_cnxt->username, "%d,%8[^\n]", &userID, username) == 2 &&
               fscanf(sendnames_Server_cnxt->status, "%d,%d", &userID, &onlineStatus) == 2)
        {
            // Check if the user is online
            if ((onlineStatus == 1) && (userID != selfUserID))
            {
                found = 1;
                if(send_packet(socket,CONFIG_PACKET,"sh",username,userID) == -1)
                {
                    debugError("send packet");
                    return -1;
                }
            }
        }
        if (found != 1)
        {
            if (send_packet(socket, CONFIG_PACKET, "sh", "NaN", -1) == -1)
            {
                debugError("send packet");
                return -1;
            }
        }
    }
    return 0;
}

/**
 * @brief Handler function for sending username
 * 
 * @param socket 
 * @param selfUserId 
 * @return int 
 */
int send_username(int socket, int selfUserId)
{
    int retval = 0;
    //Open the file descriptors
    if(fopen_db_files() == -1)
    {
        debugError("file open");
        return -1;
    }

    retval = sendOnlineUsernames(socket, selfUserId);
   

    if(fclose_db_files() == -1)
    {
        debugError("file close");
        return -1;

    }

    return retval;
}


int onlinestatus(int userID)
{
    server_cnxt *checkstatus_Server_cnxt = get_server_context();
    if (checkstatus_Server_cnxt != NULL)
    {
        // Read the file line by line
        int storedUserId, storedStatus;
    

        // Loop through the file to find the userID
        while (fscanf(checkstatus_Server_cnxt->status, "%d,%d", &storedUserId, &storedStatus) == 2)
        {
            if (storedUserId == userID)
            {
                //Match found
                if(storedStatus == 1)
                {
                    //user is online;
                    return 1;
                }
                else
                {
                    //User is offline
                    return 0;
                }
            }
            else
            {
                
            }
        }


    }
    //No such user 
    return -1;

}

int check_status(int userID)
{
    int retval = 0;
    //Open the file descriptors
    if(fopen_db_files() == -1)
    {
        debugError("file open");
        return -1;
    }

    if((retval = onlinestatus(userID)) == 1)
    {
        //Match found
    }

    if(fclose_db_files() == -1)
    {
        debugError("file close");
        return -1;

    }

    return retval;

}