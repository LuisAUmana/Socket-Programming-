#include <iostream>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main(void) {
    // WinSock that will help with ports
    WSADATA data;
    WORD ver = MAKEWORD(2, 2);
    int Start = WSAStartup(ver, &data);
    
    // Fill in structure
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // While loop for messages
    int x = 0;
    while (x < 100) {
    printf("> ");
    fgets(client_message, 512, stdin);

    // Send the message to server:
    if (sendto(socket_desc, client_message, strlen(client_message), 0,
        (struct sockaddr*)&server_addr, server_struct_length) < 0) {
        return -1;
    }

    // Receive the server's response:
    if (recvfrom(socket_desc, server_message, sizeof(server_message), 0,
        (struct sockaddr*)&server_addr, &server_struct_length) < 0) {
        return -1;
    }
    printf(">Server: %s\n", server_message);
    x = x + 1;
   
    }

    // Close the socket:
     closesocket(socket_desc);
    
    return 0;

    
}


