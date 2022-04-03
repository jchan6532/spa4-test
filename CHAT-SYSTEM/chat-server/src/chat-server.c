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
#include "../inc/processMessage.h"
#include "../inc/socketFunctions.h"

#define BUFFERSIZE 80
#define PORT 5000


MASTERLIST masterList;
bool masterListInUse;
bool messageListInUse;



/************************ TO BE DUMPED********************/
void Show(){
	MESSAGELIST* current = masterList.msgListHead;
	system("clear");
	sleep(1);
	do{
		if(current != NULL){
			printf("%s\n", current->Message);
			current = current->next;
		}
		
		if(current == NULL){
			printf("null\n");
			break;
		}
	}while(1);
	sleep(1);
	
	return;
}
/*********************************************************/



void InitializeMasterList(void){
	int i = 0;
	
	masterList.msgListHead = NULL;
	masterList.numClients = 0;
	masterList.numMsgInList = 0;
	for(i=0;i<MAXCLIENTS; i++){
		masterList.allClients[i].clientSocket = -1;
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





void EmptyClientInfo(int targetIndex){
	masterList.numClients--;
	masterList.allClients[targetIndex].clientSocket = -1;
	strcpy(masterList.allClients[targetIndex].IPAddress, "");
	strcpy(masterList.allClients[targetIndex].UserName, "");
	if(masterList.allClients[targetIndex].lastTimeUserSentMsg != NULL){
		free(masterList.allClients[targetIndex].lastTimeUserSentMsg);
	}
	masterList.allClients[targetIndex].lastTimeUserSentMsg = NULL;
	masterList.allClients[targetIndex].isActive = false;
	
	return;
}







void* DealWithClient(void* clientInfoPtr){
	CLIENTINFO clientInfo = *((CLIENTINFO*)clientInfoPtr);
	int i = 0;
	int clientSocket = 0;
	int targetClientIndex = checkExistingClients(&masterList);
	
	BusyWaitForMasterList();
	
	masterList.numClients++;
	if(targetClientIndex != NOMORESPACE){
		masterList.allClients[targetClientIndex].isActive = clientInfo.isActive;
		masterList.allClients[targetClientIndex].clientSocket = clientInfo.clientSocket;
		strcpy(masterList.allClients[targetClientIndex].UserName, clientInfo.UserName);
		strcpy(masterList.allClients[targetClientIndex].IPAddress, clientInfo.IPAddress);
		
		clientSocket = masterList.allClients[targetClientIndex].clientSocket;
	}
	
	masterListInUse = false;
	
	
	
	bool clientEndedConvo = false;
	char buffer[BUFFERSIZE] = "";
	char userName[6+2] = "";
	char senderIP[16] = "";
	char outgoingMessage[75] = "";
	int arrowIndex = 0;
	int bytesRead = 0;
	
	
	while(clientEndedConvo == false){
		memset(buffer, 0, BUFFERSIZE);
		
		bytesRead = read(clientSocket, buffer, BUFFERSIZE);
		if (bytesRead > 0) {
			printf("SENT: %s\n", buffer);
			arrowIndex = parseMessage(buffer, senderIP, outgoingMessage, userName);
			printf("OUTGOINGMESSAGE: %s\n", outgoingMessage);
			if(arrowIndex == CLIENTSAID_ADIOS){
					
				BusyWaitForMasterList();
				EmptyClientInfo(targetClientIndex);
				masterListInUse = false;
				
				clientEndedConvo = true;
			}
			else{
				BusyWaitForMasterList();
				
				
				strcpy(masterList.allClients[targetClientIndex].UserName, userName);
				
				MESSAGELIST* current = masterList.msgListHead;
				if(current == NULL){
					current = (MESSAGELIST*)calloc(1, sizeof(MESSAGELIST));
					strcpy(current->Message, outgoingMessage);
					strcpy(current->SenderIP, senderIP);
					current->arrowIndex = arrowIndex;
					masterList.msgListHead = current;
					current->next = NULL;
				}
				else{
					do{
						if(current->next == NULL){
							current->next = (MESSAGELIST*)calloc(1, sizeof(MESSAGELIST));
							current = current->next;
							strcpy(current->Message, outgoingMessage);
							strcpy(current->SenderIP, senderIP);
							current->arrowIndex = arrowIndex;
							current->next = NULL;
							break;
						}
						else{
							current = current->next;
						}
					}while(1);
				}
				
				masterListInUse = false;
				
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
	usleep(100);
	
	while(stopBroadcasting == false){
		if(masterList.numClients <= 0){
			stopBroadcasting = true;
		}
		
		if(stopBroadcasting == false){
			
			currentMsg = masterList.msgListHead;
			do{
				if(currentMsg != NULL){
					for(i=0;i<MAXCLIENTS;i++){
						if(masterList.allClients[i].isActive == true){
							if(strcmp(masterList.allClients[i].IPAddress, currentMsg->SenderIP) == 0){
								currentMsg->Message[currentMsg->arrowIndex] = '>';
								currentMsg->Message[currentMsg->arrowIndex+1] = '>';
							}
							write(masterList.allClients[i].clientSocket, currentMsg->Message, strlen(currentMsg->Message));
						}
					}
					//printf("SENT: %s\n", currentMsg->Message);
					currentMsg = currentMsg->next;
				}
				else{
					break;
				}
				usleep(1);
			}while(1);
			
			
			BusyWaitForMasterList();
			
			currentMsg = masterList.msgListHead;
			do{
				if(currentMsg != NULL){
					next = currentMsg->next;
					free(currentMsg);
					currentMsg = next;
				}
				else{
					break;
				}
			}while(1);
			
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
	
	//char tmpMessage[MAXMESSAGEBUFFER] = {};
	
	//sprintf(tmpMessage,"%d|172.168.20.22|[mikee]|AAAAAAA|AA|K+DS", 15);
	//printf("%s\n", tmpMessage);
	//parseMessage(tmpMessage);
	
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
		strcpy(connectingClient.IPAddress, inet_ntoa(clientAddress.sin_addr));
		strcpy(connectingClient.UserName, "");
		connectingClient.isActive = true;

		
		pthread_create(&(clientThreadIDs[i-1]), NULL, DealWithClient, (void*)&connectingClient);
		//usleep(100);
		if(initialBroadcast == true){
			pthread_create(&broadcastThreadID, NULL, BroadCast, NULL);
			initialBroadcast = false;
			usleep(1);
		}
		
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
	joinStatus = pthread_join(broadcastThreadID, NULL);

	close(serverSocket);



	return 0;
}
