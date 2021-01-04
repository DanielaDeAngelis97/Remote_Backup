# Remote_Backup
Programmazione di Sistema 20/21 - Project

## Summary
The project aims at building a client-server system that performs an incremental back-up of the content of a folder (and all its sub-folders) on the local computer onto a remote server. Once launched, the system will operate as a background service keeping the content of the configured folder in sync with the remote copy: whenever the monitored content changes (new files are added, existing ones are changed and/or removed), an HTTP request from client to server is sent across the network in order to replicate the changes on the other side. In case of transient errors (network portioning or failure of the remote server), the system will keep on monitoring the evolution of the local folder and try to sync again, every 30 seconds, as soon as the transient error condition vanishes).

## Aim of the project
The system consists of two different modules (the client and the server) that interact via a TCP socket connection and in asynchronous way. A dialogue takes place along this connection allowing the two parties to understand each-other, thanks to the HTTP application-level-protocol.
In order to remain synced, both the client and the server have an image of the monitored files as they are stored in the respective file system. 
In addition to the HTTP protocol, there is a function, called "*checksync*", that allows the client to verify whether or not the server already has a copy of a given file or folder. Since the filename and size are not enough to proof that the server has the same copy as the client, a (robust) hash has been computed per each file and sent to the server. Since the server may need some time to compute the answer, the protocol operates in asynchronous mode, without requiring an immediate response of the server before issuing a new request to it.

### Client Side
The client side is in charge of continuously monitoring a specific folder that can be specified by the user and check that all the contents are in sync with the sever side. To perform this operation, we create a file system watcher found here (https://solarianprogrammer.com/2019/01/13/cpp-17-filesystem-write-file-watcher-
monitor/). Whenever a discrepancy is found, the (updated) file  will be transfered to the server. 

### Server Side
The server side is responsible of listening on socket connection and accept connection requests from clients. The server is designed in order to manage more than one client and have separate backup folders for each of them. When the communication channel is setup, a check of the client identity will be performed (by exchanging email and password in encrypted form) in order to associate the connection to the proper configuration parameters. 
Per each existing connection, incoming messages will be evaluated in the order they arrive and a suitable response will be generated, either containing the requested information or an error code. In case of success, any implication of the command will be guaranteed. In case of error, no change will happen on the server
side.

## General Informations
This project can be executed both on Posix and Microsoft hosts. However, you have to change some configurations parameters:
- In *CMakeList.txt*, both in Client and in Server, you have to uncomment the part relative to your operating system and comment the other one;
- You must insert the path to watch when it is required by console, using the character "/" as separator. *Example: /Users/u1/Desktop/Folder1*;
- Server side you must insert program arguments from "*Run -> Edit Configurations... -> Program argument*" or after the executable if you run the application by terminal. The arguments are: server address, port and database folder (the folder you choose to save the backup). *Example: "127.0.0.1 1234 /Users/u1/Desktop/DB"*.

