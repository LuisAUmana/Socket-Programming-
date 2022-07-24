#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

void main()
{
	// WinSock that will help with ports
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int Start = WSAStartup(ver, &data);
	
	// Create a socket
	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	
	// Bind the ip address and port to a socket
	sockaddr_in structure;
	structure.sin_family = AF_INET;
	structure.sin_port = htons(42420);
	structure.sin_addr.S_un.S_addr = INADDR_ANY; 
	bind(server, (sockaddr*)&structure, sizeof(structure));

	// Listening socket using WinSock
	listen(server, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientPort = accept(server, (sockaddr*)&client, &clientSize);
	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << " The client is here "<< endl; 
	}
	// Close listening socket
	closesocket(server);

	// While loop: accept and echo message back to client
	char buf[4096];
	string Input;

		// Wait for client to send data
		int Response = recv(clientPort, buf, 4096, 0);
		cout << ">Client: " << string(buf, 0, Response) << endl;

		do
		{
			// Prompt the user for some text
			cout << "> ";
			getline(cin, Input);
			if (Input.size() > 0)		// Make sure the user has typed in something
			{
				// Send the message
				int Sent = send(clientPort, Input.c_str(), Input.size() + 1, 0);
				if (Sent != SOCKET_ERROR)
				{
					// Wait for response
					int Response = recv(clientPort, buf, 4096, 0);
					if (Response > 0)
					{
						// Echo response to console
						cout << ">Server: " << string(buf, 0, Response) << endl;
					}
				}
			}

		} while (Input.size() > 0);
		
		// Close the socket
		closesocket(server);

		// Cleanup winsock
		WSACleanup();


	
}