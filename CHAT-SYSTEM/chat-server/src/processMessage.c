/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"

int parseMessage(char* buffer, char* senderIP, char* outgoingMessage, char* userName)
{

	// Create some variables that are used to create constants for delimeter
	// aswell variables to store the user credentials in tempVariables.
	const char s[DELiMETERSIZE] = "|";

	char processedMessage[MAXMESSAGESIZE] = {}; //FULL MSG
	char msg[MESSAGESIZEPERPACKET] = {};
	int splitCounter = 0;


	getUserMessage(buffer, msg);
	if (strcmp(msg, "bye") == 0) {
		return CLIENTSAID_ADIOS;
	}

	//15|172.168.20.22|[mikee]|AAAAAAA|AA|K+DS
	char* token = strtok(buffer, s); // GETS MSG LEN
	splitCounter++;

	while (token != NULL)
	{
		splitCounter++;
		token = strtok(NULL, s);

		switch (splitCounter)
		{
		case 3:
			strcpy(userName, token);
			break;
		case 2:
			strcpy(senderIP, token);
			break;
		default:
			break;
		}
	}
	sprintf(outgoingMessage, "%s %s << %s (04:19:59)", senderIP, userName, msg);
	printf("%s\n", outgoingMessage);
	return strlen(senderIP) + strlen(userName) + 2;
}

void getUserMessage(char* unParsedMessage, char* parsedMsg)
{
	int messageLength = 0;
	int messageDelimCounter = 0;
	
	messageLength = strlen(unParsedMessage);
	
	for(int i = 0; i < messageLength; i++)
	{	
		if(unParsedMessage[i] == '|' && messageDelimCounter != MAXDELIMSPERMESSAGE)
		{
			messageDelimCounter++;
		}
		if(messageDelimCounter == MAXDELIMSPERMESSAGE)
		{
			strncpy(parsedMsg, unParsedMessage + i + 1, messageLength - i);
			break;
		}
	}
}

int checkExistingClients(MASTERLIST* ml)
{
	int i = 0;
	/*
	for(i = 0; i < MAXCLIENTS; i++)
	{
		if(strcmp(ml->allClients[i].IPAddress, incomingIP) == 0){
			return i;
		}
	}
	*/
	
	for(i = 0; i < MAXCLIENTS; i++)
	{
		if(ml->allClients[i].isActive == false){
			return i;
		}
	}
	
	return NOMORESPACE;
}

/*
int FindArrow(char* msg){
	int i = 0;
	for(i=0;i<strlen(msg), i++){
		if(msg[i] == '<'){
			return i;
		}
	}
	return -1;
}
*/







