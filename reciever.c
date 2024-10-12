#include <arpa/inet.h>
// This header file provides definitions for internet operations, such as converting IP addresses to a format suitable for socket communication.
#include <stdio.h>
// Standard input/output library. Provides functions like printf() for printing output and fopen() for file handling.
#include <stdlib.h>
// Provides standard utility functions such as exit() and memory management functions like malloc().
#include <string.h>
// Provides functions for handling strings, like memset() and strcmp().
#include <sys/socket.h>
// Contains definitions for socket operations, like creating a socket, connecting, sending, and receiving data.
#include <unistd.h>
// Provides access to the POSIX operating system API, including functions for file operations (close()) and other system calls.
#include <netinet/in.h>
/*This header file is part of the Berkeley sockets API and provides constants, structures, 
and functions for manipulating addresses and sockets in the IPv4 and IPv6 Internet protocol family. */
#include <stdbool.h>
/*This is a standard C library introduced in C99 that allows the use of Boolean data types (true/false) in C programs. 
Before stdbool.h, C did not have a native bool type, and programmers used integers (0 for false, non-zero for true).*/

#define CHUNK_SIZE 1024
// CHUNK_SIZE is a macro that defines the size of each chunk of data (1024 bytes) to be sent over the network in one transmission.

int main() {
    int SERVER_PORT = 8877;
    socklen_t client_address_len;

    // Setup server address

    struct sockaddr_in server_address; // This structure holds the address of the server. It contains the IP address and the port number.
    memset(&server_address, 0, sizeof(server_address)); // this clears the memory of the server_address
    server_address.sin_port = htons(SERVER_PORT); // This converts the port number server_port (8877) into network byte order  using htons().
    server_address.sin_family = AF_INET; // it indicates the use of IPv4 addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create socket
    int listen_sock;
    if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Could not create listen socket\n");
        return 1;
    }

    // Bind socket
    if ((bind(listen_sock, (struct sockaddr *)&server_address, sizeof(server_address))) < 0) {
        printf("Could not bind socket\n");
        return 1;
    }
    else{
        printf("Connected with the sender\n");
    }

    // Listen for incoming connections
    int wait_size = 16;
    if (listen(listen_sock, wait_size) < 0) {
        printf("Could not open socket for listening\n");
        return 1;
    }

    // Accept connections and process the file transmission
    while (true) {
        // Accept a connection
        struct sockaddr_in client_address;
        int sock;
        if ((sock = accept(listen_sock, (struct sockaddr*)&client_address, &client_address_len)) < 0) {
            printf("Could not open socket to accept data\n");
            return 1;
        }

        // Corrected: inet_ntoa expects `client_address.sin_addr` as the argument
        printf("Client connected with IP address: %s\n", inet_ntoa(client_address.sin_addr));

        // Open a new file to save the received video
        FILE* file = fopen("video.mp4", "wb");  // Path for saving the received video
        if (!file) {
            printf("Could not open file to write\n");
            return 1;
        }
        else{
            printf("File is being recieved\n");
        }

        // Receive data in chunks and write to the file
        char buffer[CHUNK_SIZE];
        int n;
        while ((n = recv(sock, buffer, CHUNK_SIZE, 0)) > 0) {
            if (strncmp(buffer, "EOF", 3) == 0) {  // Check for EOF marker
                break;
            }
            fwrite(buffer, 1, n, file);
        }

        // Close the file and socket
        fclose(file);
        close(sock);

        printf("File received successfully\n");
    }

    close(listen_sock);
    return 0;
}
