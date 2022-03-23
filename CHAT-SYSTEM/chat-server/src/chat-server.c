/* FILE :           chat-client.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFFERSIZE 1024
#define PORT 5000

int main(void)
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == -1) {
		return -1;
	}

	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT);

	int bindRet = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (bindRet == -1) {
		return -1;
	}

	int listenRet = listen(serverSocket, 5);
	if (listenRet == -1) {
		return -1;
	}

	//================================================================
	struct sockaddr_in clientAddress;
	int clientLen = 0;
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
	if (clientSocket == -1) {
		return -1;
	}

	char buffer[BUFFERSIZE];
	memset(buffer, 0, BUFFERSIZE);

	int bytesRead = 0;
	bytesRead = read(clientSocket, buffer, BUFFERSIZE);
	if (bytesRead > 0) {
		printf("%s\n", buffer);
	}

	close(serverSocket);

	return 0;
}
