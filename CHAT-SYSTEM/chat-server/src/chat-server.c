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
#include <stdbool.h>
#include <pthread.h>

#include "../inc/MasterList.h"

#define BUFFERSIZE 1024
#define PORT 5000


MASTERLIST masterList;
bool masterListInUse;

void InitializeMasterList(void){
	int i = 0;
	
	masterList.msgListHead = NULL;
	masterList.numClients = 0;
	masterList.numMsgInList = 0;
	masterList.highestClientID = 0;
	for(i=0;i<MAXCLIENTS; i++){
		masterList.allClients[i].clientSocket = -1;
		masterList.allClients[i].clientID = -1;
		strcpy(masterList.allClients[i].IPAddress, "");
		strcpy(masterList.allClients[i].UserName, "");
		masterList.allClients[i].lastTimeUserSentMsg = NULL;
		masterList.allClients[i].isActive = false;
	}
	
	masterListInUse = false;
	return;
}

void* DealWithClient(void* clientSocketPtr){
	bool clientEndedConvo = false;
	char buffer[BUFFERSIZE];
	int clientSocket = *((int*)clientSocketPtr);
	
	while(clientEndedConvo == false){
		
		memset(buffer, 0, BUFFERSIZE);
		
		do{
			if(masterListInUse == false){
				masterListInUse = true;
				break;
			}
			usleep(1);
		}while(masterListInUse == true);
		
		int bytesRead = 0;
		bytesRead = read(*clientSocket, buffer, BUFFERSIZE);
		if (bytesRead > 0) {
			printf("%s\n", buffer);
			if(strcmp(buffer, "bye") == 0){
				clientEndedConvo = true;
				masterList.numClients--;
			}
		}
		else{
			return (void*)(-1);
		}
		
		masterListInUse = false;
	}
	
	return (void*)1;
}

void* BroadCast(void){
	bool stopBroadcasting = false;
	int i = 0;
	MESSAGELIST* currentMsg = NULL;
	MESSAGELIST* previousMsg = NULL;
	
	while(stopBroadcasting == false){
		if(masterList.numClients == 0){
			stopBroadcasting = true;
		}
		
		if(stopBroadcasting == false){
			currentMsg = masterList.msgListHead;
			while(currentMsg != NULL){
				for(i=0;i<MAXCLIENTS;i++){
					if(masterList.allClients[i].isActive == true){
						write(masterList.allClients[i].clientSocket, currentMsg->Message, strlen(currentMsg->Message));
					}
				}
				currentMsg = currentMsg->next;
			}
			
			do{
				if(masterListInUse == false){
					masterListInUse = true;
					break;
				}
				usleep(1);
			} while(masterListInUse == true);
			
			currentMsg = masterList.msgListHead;
			while(currentMsg != NULL){
				previousMsg = currentMsg;	
				free(previousMsg);
				previousMsg = NULL;
			}
			
			masterList.msgListHead = NULL;
			masterListInUse = false;
			usleep(1);
		}
	}
	
	return (void*)1;
}
	

int main(void)
{
	
	/*
	* SETUP THE MASTERLIST
	*/
	InitializeMasterList();
	
	
	
	
	/*
	* SETUP SOCKET
	*/
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





	/*
	* PROCESSING LOOP FOR TAKING IN CLIENTS
	*/
	struct sockaddr_in clientAddress;
	int clientLen = 0;
	bool stopAcceptClient = false;
	bool initialBroadcast = true;
	pthread_t clientThreadIDs[MAXCLIENTS];
	pthread_t broadcastThreadID;

	int i = 0;
	
	while(stopAcceptClient == false){
		
		clientLen = sizeof(clientAddress);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
		if (clientSocket == -1) {
			return -1;
		}
		printf("connected to %s \n", inet_ntoa(clientAddress.sin_addr));
		

		for(i=0;i<MAXCLIENTS;i++){
			if(masterList.allClients[i].clientID == -1){
				masterList.allClients[i].clientID = masterList.highestClientID++;
			}
			
			if(strcmp(masterList.allClients[i].IPAddress, "") == 0){
				strcpy(masterList.allClients[i].IPAddress, inet_ntoa(clientAddress.sin_addr));
			}
			
			if(strcmp(masterList.allClients[i].UserName, "") == 0){
				strcpy(masterList.allClients[i].UserName, "admin");
			}
			
			if(masterList.allClients[i].isActive == false){
				masterList.allClients[i].isActive = true;
			}
		}
		masterList.numClients++;
		masterList.highestClientID++;
		
		pthread_create(&(clientThreadIDs[masterList.numClients-1]), NULL, DealWithClient, (void*)&clientSocket);
		if(initialBroadcast == true){
			pthread_create(&broadcastThreadID, NULL, BroadCast, NULL);
			initialBroadcast = false;
		}
		
		if(masterList.numClients == 0){
			stopAcceptClient = true;
		}
		else if(masterList.numClients == MAXCLIENTS){
			stopAcceptClient = true;
		}
	}
	
	int joinStatus = 0;
	for(i=0;i<MAXCLIENTS;i++){
		joinStatus = pthread_join(clientThreadIDs[i], NULL);
	}
	joinStatus = pthread_join(broadcastThreadID, NULL);



	close(serverSocket);

	return 0;
}
