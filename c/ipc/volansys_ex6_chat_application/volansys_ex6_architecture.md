# Server Implementation
Will be using Async Implementation method for the application - making client client socketes non blocking and using select with that

## Server Objects and Methods
1. Databases:
    Non encrypted text files with csv formatting
    a. username database: <UserID>,<Username><LF>
    b. password database: <UserID>,<Password><LF>
    c status Database: <UserID>,<Status><LF>

    ### Database Access method style
    a. Get the Server Context, - static context. open File descriptors for all databases in server context
    b. Perform the Data base operations
    c. Close all the file descriptors in server context

2. Server Context:
    A struct stored as a static variable which stores critical data related to server

    ### Server Context Access method style
    a. Call get context function which returns the Server context,
    b. Perform the operation
    This method ensures that only one context of Server data structure is there

3. User Context: 
    It is created when a thread receives a new connection from the listener
    a. Max 128 Users can signup for the server

    ### User Context Access Method style
    a. Each User Context is unique to its own thread, just care is taken when passing user context from listener to thread - safeguarding using mutexes and condition variable

## Client Server Communication
    a. Application level Protocol:
        i. Max Protocol Packet len: 140 bytes

    |PacketType-1Byte|Body Len-1Byte|Body-Max 138 Bytes.........................|

    b. Packet Types:
        i. Config Packets
        Example of Config Packet from Client to Server
            |0x01|0x06|04User| 
                A string is always prefixed with a 2 byte len field to help unpack it
        Example of Config Packet from Server to Client
            |0x01|0x07|04User1|
                above packet body consists of Username (User) and UserID(1)  
        ii. Chat Init Packets
            |0x02|0x01|3|
                Client Sends this packet to server to initiate chat with User ID 3 - Here Server gets to know with whom Client is chatting, all the messages from client will be forwared to thread with this UserID
        iii. Message Packets
            |0x03|0x07|05Hello|
                Client to Server Packet
            |0x03|0x0A|10User:Hello|
                Server to Client
        iv. Error Packets
            |0x04|0x14|20User Already Online!|


## MultiThreads Description
    a. 21 Threads running on the Server: Main thread + 20 Client Threads
    b. Main thread runs the listeners, It accepts new connections, creates a new User Context and enqueues this information into a linked list.
    c. Client Threads dequeues this Context and each thread handles the particular client
    d. Thus concurrently the Server can handle 20 Users
    e. Another Message Queue is used to relay message from one thread to another (Inter thread Communication)

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
3. Create a thread pool (Max size kept 20), which keeps on polling a queue for new incoming connections
4. Start the Server Listener on the main thread

## Create a Server listening for connections from Client 
1. Client Connects with the Server, listener thread on server accepts the connection and passes the Context to thread to handle the connection
2. Use new thread for new client
    a. Need to have appropriate synchronization mechanisms in place - for safeguarding common resources, synchronization and concurrency - Mutexes along with Condition Variable

## Client Thread Handles the Connection henceforth
1. Client immediately sends Username to the Server - Client thread handles this -
    a. If new Username: Create a new entry in 2 databases - Username and Status
    b. If existing Username - Just update the status to ONLINE in Status Database
2. Upon Successful handling by the Server, it replies the client with a list of UserIds and Usernames Online right now on Server
3. The Client the replies with the UserID to start chat with to the server
4. Server receives this request, and validates if the UserID is online or not, if not, it replies with Error Packet to the Client. Server will henceforth forward all the messages of this client to this UserId unless a new Chat init packet with new UserID is received
5. If the UserID mentioned above is online, server starts receiving Message Packets.

## Messaging Mechanism
1. Message Queue is implemented for this purpose
Below steps explain the messaging mechanism at server side:
    a. Client Sends a message to Server, Client handling thread on server receives it, appends this message with Destination user ID and enqueues this message in Message Queue
    b. All the Client handling threads which wait on this queue read the UserID:
        i. If doesnot matches their own UserID, they ignore the message
        ii. It UserID matches, then the Destination client thread dequeues the Message
    c. The dequeued message is then written to the Client Socket as a Message Packet
