/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"
#define MAXDELIMSPERMESSAGE 4
#define MAXMESSAGESIZE 81
#define IPLENGTH 16
#define MAXBYTESRECORDED 3
#define DELiMETERSIZE 3

int parseMessage(char* message)
{

	// Create some variables that are used to create constants for delimeter
	// aswell variables to store the user credentials in tempVariables.
	const char s[DELiMETERSIZE] = "|";
	char* token;
	
	char msgLength[MAXBYTESRECORDED] = {};
	char userIp[IPLENGTH] = {};
	char directionOfMessage[4] = {};
	char msg[41] = {};
	char user[8] = {};
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


