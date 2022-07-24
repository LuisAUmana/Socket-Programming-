#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 42420;						// Listening port # on the server

	// WinSock that will help with ports
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int Start = WSAStartup(ver, &data);
	
	// Create socket
	SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
	
	// Fill in structure
	sockaddr_in structure;
	structure.sin_family = AF_INET;
	structure.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &structure.sin_addr);

	// Connect to server
	int connResult = connect(client, (sockaddr*)&structure, sizeof(structure));
	// A function that will give the user a message if the server is not turned on
	if (connResult == SOCKET_ERROR)
	{
		cerr << "The server is currently Unavailable" << endl;
		closesocket(client);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string Input;

	do
	{
		// Obtains whatever the user typed in
		cout << "> ";
		getline(cin, Input);
		if (Input.size() > 0)		// Checks if user has typed something
		{
			// Send the message
			int Sent = send(client, Input.c_str(), Input.size() + 1, 0);
			if (Sent != SOCKET_ERROR)
			{
				// Wait for response
				int Response = recv(client, buf, 4096, 0);
				if (Response > 0)
				{
					// Echo response to console
					cout << ">Server: " << string(buf, 0, Response) << endl;
				}
			}
		}

	} while (Input.size() > 0);

	// Close the socket
	closesocket(client);

	// Cleanup winsock
	WSACleanup();

}
