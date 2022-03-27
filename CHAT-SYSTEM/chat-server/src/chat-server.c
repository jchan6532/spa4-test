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
#include <stdlib.h>

#include "../inc/MasterList.h"

#define BUFFERSIZE 80
#define PORT 5000


MASTERLIST masterList;
bool masterListInUse;
bool messageListInUse;




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





void BusyWaitForMasterList(void){
	do{
		if(masterListInUse == false){
			masterListInUse = true;
			break;
		}
		usleep(1);
	} while(masterListInUse == true);
	
	return;
}







void* DealWithClient(void* clientInfoPtr){
	int i = 0;
	int clientSocket = 0;
	int targetClientIndex = 0;
	CLIENTINFO clientInfo = *((CLIENTINFO*)clientInfoPtr);	//MAYBE SPLIT CLIENTINFO INTO SEPARATE PRIMITIVE DATATYPE VARIABLES, AND ASSIGN 1 BY 1
	
	BusyWaitForMasterList();
	
	masterList.numClients++;
	masterList.highestClientID++;
	for(i=0;i<MAXCLIENTS;i++){
		if(masterList.allClients[i].isActive == false){
			targetClientIndex = i;
			masterList.allClients[i].isActive = clientInfo.isActive;
			masterList.allClients[i].clientID = clientInfo.clientID;	//QUESTIONABLE
			masterList.allClients[i].clientSocket = clientInfo.clientSocket;
			strcpy(masterList.allClients[i].UserName, clientInfo.UserName);
			strcpy(masterList.allClients[i].IPAddress, clientInfo.IPAddress);
			
			clientSocket = masterList.allClients[i].clientSocket;
			break;
		}
	}
	
	masterListInUse = false;
	
	
	
	bool clientEndedConvo = false;
	char buffer[BUFFERSIZE];
	char userName[6] = "";
	int bytesRead = 0;
	char clientMessage[41];
	
	while(clientEndedConvo == false){
		memset(buffer, 0, BUFFERSIZE);
		memset(clientMessage, 0, 41);
		
		bytesRead = read(clientSocket, buffer, BUFFERSIZE);
		if (bytesRead > 0) {
			//printf("%s\n", buffer);
			if(strcmp(buffer, "bye") == 0){
					
				BusyWaitForMasterList();
				
				masterList.numClients--;
				/*CLEAR CLIENT LIST*/
				
				masterListInUse = false;
				clientEndedConvo = true;
			}
			else{
				for(i=0;i<MAXCLIENTS;i++){
					if(masterList.allClients[i].isActive == true){
						write(masterList.allClients[i].clientSocket, buffer, strlen(buffer));
					}
				}
					
				// BusyWaitForMasterList();
				
				// /*NEED TO PARSE USERNAME HERE AND GET MESSAGE CONTENT*/
				// //strcpy(masterList.allClients[targetClientIndex].UserName, userName);
				
				// /*ADD PARSED CLIENT MESSAGE TO THE LINKED LIST*/
				// MESSAGELIST* current = masterList.msgListHead;
				// do{
				// 	if(current == NULL){
				// 		current = (MESSAGELIST*)calloc(1, sizeof(MESSAGELIST));
				// 		strcpy(current->Message, buffer);
				// 		current->next = NULL;
				// 		//printf("added %s\n", current->Message);
				// 	}
				// 	current = current->next;
				// } while(current != NULL);
				
				// masterListInUse = false;
				
			}
		}
	}
	
	pthread_exit((void*)1);
	
	return (void*)1;
}








void* BroadCast(void* data){
	bool stopBroadcasting = false;
	int i = 0;
	MESSAGELIST* currentMsg = NULL;
	MESSAGELIST* next = NULL;
	
	while(stopBroadcasting == false){
		if(masterList.numClients <= 0){
			stopBroadcasting = true;
		}
		
		if(stopBroadcasting == false){
			currentMsg = masterList.msgListHead;
			do{
				if(currentMsg != NULL){
					printf("%s\n", currentMsg->Message);
					for(i=0;i<MAXCLIENTS;i++){
						if(masterList.allClients[i].isActive == true){
							write(masterList.allClients[i].clientSocket, currentMsg->Message, strlen(currentMsg->Message));
						}
					}
					currentMsg = currentMsg->next;
				}
				usleep(1);
			}while(currentMsg != NULL);
			
			BusyWaitForMasterList();
			
			currentMsg = masterList.msgListHead;
			do{
				if(currentMsg != NULL){
					next = currentMsg->next;
					free(next);
					currentMsg = next;
				}
			}while(currentMsg != NULL);
			
			masterList.msgListHead = NULL;
			masterListInUse = false;
			
			usleep(1);
		}
	}
	
	pthread_exit((void*)1);
	
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


	/*
	* BIND SOCKET
	*/
	int bindRet = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (bindRet == -1) {
		return -1;
	}

	/*
	* LISTEN SOCKET
	*/
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

	int i = 0; //I IS NUMBER OF ACCEPTED CLIENTS
	CLIENTINFO connectingClient = {0};
	while(stopAcceptClient == false){
		
		clientLen = sizeof(clientAddress);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
		if (clientSocket == -1) {
			return -1;
		}
		printf("connected to %s \n", inet_ntoa(clientAddress.sin_addr));
		i++;
		
		connectingClient.clientSocket = clientSocket;
		connectingClient.clientID = masterList.highestClientID + 1;
		strcpy(connectingClient.IPAddress, inet_ntoa(clientAddress.sin_addr));
		strcpy(connectingClient.UserName, "null");
		connectingClient.isActive = true;

		
		pthread_create(&(clientThreadIDs[i-1]), NULL, DealWithClient, (void*)&connectingClient);
		usleep(10);
		// if(initialBroadcast == true){
		// 	pthread_create(&broadcastThreadID, NULL, BroadCast, NULL);
		// 	initialBroadcast = false;
		// 	usleep(1);
		// }
		
		if(i == 0){
			stopAcceptClient = true;
		}
		else if(i == MAXCLIENTS){
			stopAcceptClient = true;
		}
		
		//usleep(1);
	}
	
	
	
	/*
	* JOINING ALL THREADS
	*/
	int joinStatus = 0;
	for(i=0;i<MAXCLIENTS;i++){
		joinStatus = pthread_join(clientThreadIDs[i], NULL);
	}
	//joinStatus = pthread_join(broadcastThreadID, NULL);

	close(serverSocket);

	return 0;
}
