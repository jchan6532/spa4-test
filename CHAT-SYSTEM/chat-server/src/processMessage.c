/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"
#define maxDelimsPerMessage 4

int parseMessage(char* message)
{
	const char s[2] = "|";
	char* token;
	
	
	char msgLength[3] = {};
	char userIp[16] = {};
	char directionOfMessage[4] = {};
	char msg[41] = {};
	char user[8] = {};
	int messageRead = 0;
	int splitCounter = 0;
	int messageLength = 0;
	int messageDelimCounter = 0;
	
	messageLength = strlen(message);
	
	for(int i = 0; i < messageLength; i++)
	{	
		if(message[i] == '|' && messageDelimCounter != maxDelimsPerMessage)
		{
			messageDelimCounter++;
			printf("%d HEY\n", messageDelimCounter);
		}
		if(messageDelimCounter == maxDelimsPerMessage)
		{
			printf("%c \n", message[i + 1]);
			strncpy(msg, message + i + 1, messageLength - i);
			break;
		}
	}
	
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


