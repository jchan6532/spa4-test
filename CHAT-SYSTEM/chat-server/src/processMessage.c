/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"


int parseMessage(char* message)
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
				// DO some stuff
				strcpy(userIp, token);
				// with this ip check to see if the client exists
				printf("%s %d\n", token, splitCounter);
				break;
			case 3:

				strcpy(user, token);
				printf("%s %d\n", token, splitCounter);
				// DO some stuff
				break;
			case 4:
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
			printf("%d HEY\n", messageDelimCounter);
		}
		if(messageDelimCounter == MAXDELIMSPERMESSAGE)
		{
			printf("%c \n", message[i + 1]);
			strncpy(msg, message + i + 1, messageLength - i);
			break;
		}
	}
}

int checkExistingCLients(char* incomingIP, MASTERLIST* ml)
{
	for(int i = 0; i < MAXCLIENTS; i++)
	{
		if(strcmp(ml->CLIENTINFO[i], incomingIP) == 0 && ml->CLIENTINFO[i].isActive != OFFLINE)
		{
			// this means the ip exists in the user is returning 
		}
		// OFFLINE MIGHT NOT EXIST IF NOT ADD IT
		else if(ml->CLIENTINFO[i].isActive != OFFLINE)
		{
			// THIS IS CLIENT DOES NOT EXIST SO ADD IT TO THE MASTERLIST
		}
	}
	
}








