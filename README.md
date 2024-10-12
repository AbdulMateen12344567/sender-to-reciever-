# Assumptions

## Video Format: 
The video file being transmitted is assumed to be in a format that can be opened and read as a binary file (e.g., .mp4).
## File Location: 
The video file is located in the same directory as the program, or its absolute path is known and correctly specified in the code.
## Network Environment: 
Both sender and receiver are on the same network, or the network configuration allows for socket communication over TCP/IP.
## End of Transmission Marker: 
The string "EOF" is used as the marker to signify the end of the file transmission.
## Receiver Program Running: 
The receiver program should be started before the sender program to ensure it can accept the connection.
## Port Availability: 
The chosen port (8877) is available and not blocked by firewalls or other processes.

# Approach

## Sender Program:

Reads the video file in chunks of 1024 bytes (defined by CHUNK_SIZE).

Establishes a TCP connection to the receiver using the socket(), connect() system calls.

Sends the video file in chunks using the send() function until the entire file is transmitted.

Sends the "EOF" marker after the file has been transmitted to indicate the end of the file.

Closes the socket and file after successful transmission.

## Receiver Program:

Sets up a TCP listening socket using socket(), bind(), and listen() system calls.

Accepts incoming connections using the accept() call and prints the client's IP address.

Creates a new video file to write the received data.

Continuously reads data from the socket using recv() and writes it to the file in chunks.
        Stops receiving data when the "EOF" marker is detected, signaling the end of the transmission.

Closes the file and the socket after receiving the complete file.

# Challenges

## Handling EOF: 
A key challenge was identifying the end of the file transmission. To resolve this, the "EOF" marker was used as an indicator. Any alternative method would require more robust error checking or protocol design.
## File Integrity: 
Ensuring that the file is received without corruption during transmission, especially in large files, required managing chunk sizes and handling partial reads/writes effectively.
## Network Latency/Errors: 
TCP inherently handles some network issues, but ensuring that the program works in different network environments was a consideration.
## Socket Management: 
Proper handling of socket creation, binding, and closure was crucial to avoid resource leaks or connection issues.
## Compatibility: 
Ensuring compatibility between different Linux systems and keeping the code simple enough to work in a variety of setups was a challenge during the design phase.
