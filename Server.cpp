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
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);

    // Clean buffers:
   memset(server_message, '\0', sizeof(server_message));
   memset(client_message, '\0', sizeof(client_message));

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return -1;
    }
    printf("Done with binding\n");

    // While loop for recieve and send
    int x = 0;
    while (x < 100) {
        // Receive client's message:
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
            (struct sockaddr*)&client_addr, &client_struct_length) < 0) {
            return -1;
        }
       
        printf(">Client: %s\n", client_message);

        // Get input from the user:
        printf("> ");
        fgets(server_message, 512, stdin);

        if (sendto(socket_desc, server_message, strlen(server_message), 0,
            (struct sockaddr*)&client_addr, client_struct_length) < 0) {
            return -1;
        }
        x = x + 1;
      
    }
    // Close the socket:
     closesocket(socket_desc);

    return 0;
}

