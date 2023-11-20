/**
 * @file app_db.h
 * @author rohit S
 * @brief Header file for database code
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef APP_DB_H
#define APP_DB_H

#define ONLINE  1
#define OFFLINE 0

int fopen_db_files();
int fclose_db_files();
int username_handling(unsigned char* );
int status_handling(int, int);
int send_username(int, int);

#endif