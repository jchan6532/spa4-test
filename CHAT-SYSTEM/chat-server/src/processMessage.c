/* FILE :           processMessage.c
 * PROJECT :        SENG2030 - Assignment #4
 * PROGRAMMER :    	Justin Chan, Michael Dremo, Paige Lam & Erica Luksts
 * FIRST VERSION :  2022-03-18
 * DESCRIPTION :    Temp description
*/

#include "../inc/processMessage.h"

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
	
	token = strtok(message, s);
	strcpy(msgLength, token);
	splitCounter++;
	printf("%s %d\n", token, splitCounter);
	while(token != NULL)
	{
		
			if(messageRead != 1)
			{	 
				splitCounter++;	
				token = strtok(NULL, s);	
				
				//printf("%s", token);	
				
			}
			else
			{
				break;
			}		
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
			case 5:
				strcpy(msg, token);
				printf("%s %d\n", token, splitCounter);
				messageRead = 1;
				break;
			default:
				break;
			 // DO SOME STUFF
			 				
		}
		// use the ip and search through DS to check if the ip is from a reoccuring 
		// client if the ip is not known to server create it if it we have space on the server
	}
	
	splitCounter = 0;
	
	return 0;
}


