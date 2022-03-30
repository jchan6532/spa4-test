/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"


int parseMessage(char* message, MASTERLIST* ml, char* IncomingIp, char* outgoingMessage)
{

	// Create some variables that are used to create constants for delimeter
	// aswell variables to store the user credentials in tempVariables.
	const char s[DELiMETERSIZE] = "|";
	char* token;
	
	char msgLength[MAXBYTESRECORDED] = {};
	char userIp[IPLENGTH] = {};
	char directionOfMessage[MESSAGEDIRECTIONBUFFER] = {};
	char msg[MESSAGESIZEPERPACKET] = {};
	char user[MAXUSERLENGTH] = {};
	char processedMessage[MAXMESSAGESIZE] = {};
	int messageRead = 0;
	int splitCounter = 0;
	
	getUserMessage(message, msg);
	
	token = strtok(message, s);
	strcpy(msgLength, token);	
	splitCounter++;
	
	printf("%s %d\n", token, splitCounter);
	while(token != NULL)
	{
 		splitCounter++;	
		token = strtok(NULL, s);	

		switch(splitCounter)
		{
			
			case 2:
				strcpy(user, token);
				printf("%s %d\n", token, splitCounter);
				// DO some stuff
				break;
			case 3:
				strcpy(directionOfMessage, token);
				printf("%s %d\n", token, splitCounter);
				// DO some stuff
				break;
			default:
				break;
			 // DO SOME STUFF

		}
	}
	printf("the message is %s", msg);
	sprintf(outgoingMessage,"%s %s << %s (04:19:59)", IncomingIp, user, msg); 		
	splitCounter = 0;
	
	return 0;
}

void getUserMessage(char* message, char* msg)
{
	int messageLength = 0;
	int messageDelimCounter = 0;
	
	messageLength = strlen(message);
	
	for(int i = 0; i < messageLength; i++)
	{	
		if(message[i] == '|' && messageDelimCounter != MAXDELIMSPERMESSAGE)
		{
			messageDelimCounter++;
		}
		if(messageDelimCounter == MAXDELIMSPERMESSAGE)
		{
			strncpy(msg, message + i + 1, messageLength - i);
			break;
		}
	}
	printf("%s\n", msg);
}

int checkExistingCLients(char* incomingIP, MASTERLIST* ml)
{
	for(int i = 0; i < MAXCLIENTS; i++)
	{
		 if(strcmp(ml->allClients[i].IPAddress, incomingIP) != 0 && ml->allClients[i].isActive == false)
		{
			//CHECK IF THERE IS SPACE ADD IT TO THE SERVER
			// THIS IS CLIENT DOES NOT EXIST SO ADD IT TO THE MASTERLIST
		}
	}
	
}








