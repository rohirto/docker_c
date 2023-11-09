# Server Implementation
Will be using Async Implementation method for the application - libevent
Reference for LibEvent 2.0 - [https://libevent.org/libevent-book/]
## Initialization of Server 
1. Load the Dynamic Database - The dynamic database contains following parameters - Past Client Data [Username, Password, Chats with other users]
    a. First time creation of data base -
        i. Will go for File IO for Persistent Data storage (Other options sqlite3)
        ii. 1 File will be user data file - where userID and username will be stored in key-value pair, 
        iii. 2 File will be password file - where userID and password will be stored in key
        iv. 3 File will store status file - where userID and online/offline status of the user will be stored dynamically - all Offline initially
2. Method to write to file during initialization
    a. Open the file (If not there then create)
    b. Perform the operation
    c. Close the file
## Create a Server listening for connections from Client 
1. If new Client Connection, then ask for username, if new username, create a new entry in database, else if existing username - ask for password, then open a console showing him his previous chats
2. Associate the thread id with the userId in the database
3. Update the entries in the Database - Method same as first init
    a. Username - <UserID>-<Username>
    b. Password - <UserID>-<Password>
    c. Status - <UserID>-<Status>
4. Use new thread for new client
    a. Need to have appropriate synchronization mechanisms in place - for safeguarding common resources, synchronization and concurrency

5. Handling Data from the Client
    a. Use of Socket to receive the data from Client
    b. User Protocol - <Header - 0x01><Username - 8bytes><Message - max 128 bytes>
6. Inter - Thread Communication
7. Sending Data to Client
